#include "vqbinstancesform.h"
#include "ui_vqbinstancesform.h"
#include "vqbmainwindow.h"
#include "vqbglobal.h"
#include "querythread.h"
#include "completerlineedit.h"

#include </home/kde-devel/kde/include/kcombobox.h>
#include </home/kde-devel/kde/include/klineedit.h>
#include </home/kde-devel/kde/include/keditlistbox.h>
#include </home/kde-devel/kde/include/krandom.h>
#include </home/kde-devel/kde/include/kdebug.h>

#include <QRegExp>
#include <QTimer>
#include <QCompleter>

VqbInstancesForm::VqbInstancesForm(VqbMainWindow *parent) :
    VqbForm(parent),
    m_ui(new Ui::VqbInstancesForm)
{
    m_ui->setupUi(this);
    init();
}

VqbInstancesForm::~VqbInstancesForm()
{
    delete m_ui;
}

/******* Slots for GUI events **************/

void VqbInstancesForm::updateCurrentTriple()
{
    QString triple;
    triple = m_ui->cbSubject->currentText() + " " + m_ui->cbPredicate->currentText() + " ";
    triple += VqbGlobal::constructObject(m_ui->checkBoxFilter->isChecked(),
                                         m_ui->cbRelation->currentText(),
                                         m_ui->cbObject->currentText(),
                                         m_ui->cbType->currentText());


    m_ui->listBoxConditions->lineEdit()->setText(triple);
    m_currentTriple = triple;

    //update completers for comboboxes
    updateCompleters();
}

void VqbInstancesForm::updateTypes()
{
    if(m_ui->checkBoxFilter->isChecked()) {
        m_ui->cbType->setEnabled(false);
        return;
    }

    m_ui->cbType->setEnabled(true);
    QString text = m_ui->cbObject->currentText();
    m_ui->cbType->clear();
    QStringList types = VqbGlobal::literalTypes();

    if(!text.isEmpty()) {
        QRegExp rx;
        foreach(QString type, types) {
            rx.setPattern(VqbGlobal::typeRegExp(type));
            if ( !rx.exactMatch( text ) ) {
                //kDebug() << "Removing:";
                types.removeAt( types.indexOf( type ) );
            }
        }
    }
    m_ui->cbType->clear();
    m_ui->cbType->insertItems(-1, types);
}


void VqbInstancesForm::updateCompleters()
{
    QString query = "SELECT * WHERE { " + m_currentTriple + " } LIMIT 1";

    if( query == m_lastQuery ) { //don't run same query many times
        return;
    }// else
    m_lastQuery = query;
    //kDebug() << "####### " << query;

    //FIXME?: launch on a different thread, maybe
    colorLineEdits(QueryThread::countQueryResults(query));

    QString varName = VqbGlobal::randomVarName();  
    QString subject = m_ui->cbSubject->currentText();
    QString predicate = m_ui->cbPredicate->currentText();
    QString object = VqbGlobal::constructObject(m_ui->checkBoxFilter->isChecked(),
                                         m_ui->cbRelation->currentText(),
                                         m_ui->cbObject->currentText(),
                                         m_ui->cbType->currentText());

    //query for SUBJECT autocompletion
    query = varName + " " + predicate + " " + object;
    QCompleter *c;// = new QCompleter(QueryThread::queryResults(query, varName), this);
    /*c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    m_ui->cbSubject->lineEdit()->setCompleter(c);*/
    ((CompleterLineEdit *) m_ui->cbSubject->lineEdit())->addItems(QueryThread::queryResults(query, varName));

    //query for PREDICATE autocompletion
    query =  subject + " " + varName + " " + object;
    c = new QCompleter(QueryThread::queryResults(query, varName), this);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui->cbPredicate->lineEdit()->setCompleter(c);

    //query for OBJECT autocompletiono
    query = subject + " " + predicate + " " + varName;
    c = new QCompleter(QueryThread::queryResults(query, varName), this);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui->cbObject->lineEdit()->setCompleter(c);

/*    //generate random name    
    setCompletersOS( qTh->runQuery( query, varName ) );
    */
}
/******** Utility functions *************/

void VqbInstancesForm::colorLineEdits(bool hasResults)
{
    QRegExp rx(VqbGlobal::typeRegExp("var"));

    //init the three LineEdits
    QLineEdit *lS = m_ui->cbSubject->lineEdit();
    QLineEdit *lP = m_ui->cbPredicate->lineEdit();
    QLineEdit *lO = m_ui->cbObject->lineEdit();

    //init the coloring Palettes
    QPalette paletteColored( lO->palette() );//palette adjusted to given color
    paletteColored.setColor( QPalette::Base, hasResults ? Qt::white : Qt::red );
    QPalette paletteWhite( lO->palette() );//white palette
    paletteWhite.setColor( QPalette::Base, Qt::white );

    //perform the coloring
    if ( !rx.exactMatch( lS->text() ) ) {
        lS->setPalette(paletteColored);
    } else { //variables are not colored
        lS->setPalette(paletteWhite);
    }
    if ( !rx.exactMatch( lP->text() ) ) {
        lP->setPalette(paletteColored);
    } else { //variables are not colored
        lP->setPalette(paletteWhite);
    }
    if (!(rx.exactMatch( lO->text() ) && m_ui->cbType->currentText() == "var" && !m_ui->checkBoxFilter->isChecked())) { //only non-variables are colored
        lO->setPalette(paletteColored);
    } else { //color variables with white
        lO->setPalette(paletteWhite);
    }
}

/********** Initialization *************/


void VqbInstancesForm::init()
{
    //LineEdits
    m_ui->cbSubject->setLineEdit(new CompleterLineEdit(m_ui->cbSubject));

    //triple updating signals
    //connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
    connect(m_ui->cbSubject->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbSubject, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    //connect(m_ui->cbPredicate, SIGNAL(editTextChanged(QString)),
    connect(m_ui->cbPredicate->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbPredicate, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    //connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
    connect(m_ui->cbObject->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbObject, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbRelation, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbType, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->checkBoxFilter, SIGNAL(stateChanged(int)),
            this, SLOT(updateCurrentTriple()));\

    //type updating signal
    connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateTypes()));
    connect(m_ui->checkBoxFilter, SIGNAL(toggled(bool)),
            this, SLOT(updateTypes()));

    //initializations
    m_ui->cbSubject->lineEdit()->setText("?s");
    m_ui->cbPredicate->lineEdit()->setText("?p");
    m_ui->cbObject->lineEdit()->setText("?o");
    m_ui->cbRelation->insertItems(0, QStringList() << "equals" << "contains");

    //initial GUI population
    updateTypes();
    updateCurrentTriple();
}


void VqbInstancesForm::updateVars()
{
    //FIXME: add to comboboxes and completers
    //update m_varList;
}


#include "vqbinstancesform.moc"
