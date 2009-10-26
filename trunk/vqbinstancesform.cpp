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
#include </home/kde-devel/kde/include/kcompletion.h>

#include <QRegExp>
#include <QTimer>
#include <QCompleter>

VqbInstancesForm::VqbInstancesForm(VqbMainWindow *parent) :
    VqbForm(parent),
    m_ui(new Ui::VqbInstancesForm)
{
    m_ui->setupUi(this);
    init();
    kDebug() << "Init done";
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
    //FIXME: connect this signal separately, to allow firing updateCurrentTriple on KLineEdit::editTextChanged()
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
    if( m_currentTriple == m_lastTriple ) { //nothing's changed
        return;
    }// else
    m_lastTriple = m_currentTriple;

    QString query = "SELECT * WHERE { " + m_currentTriple + " } LIMIT 1";

    colorLineEdits(QueryThread::countQueryResults(query));

    QString varName = "?slot";
    QString subject = m_ui->cbSubject->currentText();
    subject = subject.isEmpty() ? VqbGlobal::randomVarName() : subject;
    QString predicate = m_ui->cbPredicate->currentText();
    predicate = predicate.isEmpty() ? VqbGlobal::randomVarName() : predicate;
    QString object = m_ui->cbObject->currentText();
    object = object.isEmpty() ? VqbGlobal::randomVarName() :
                                VqbGlobal::constructObject(m_ui->checkBoxFilter->isChecked(),
                                                           m_ui->cbRelation->currentText(),
                                                           m_ui->cbObject->currentText(),
                                                           m_ui->cbType->currentText());

    kDebug() << "--------------------------- Completion query: " << subject << predicate << object;

    //autocompletion
    query = varName + " " + predicate + " " + object + " . " + m_queryPart;
    m_queryThreads[0]->startIncrementalQuery(query, varName);

    query =  subject + " " + varName + " " + object + m_queryPart;
    m_queryThreads[1]->startIncrementalQuery(query, varName);

    query = subject + " " + predicate + " " + varName + m_queryPart;
    m_queryThreads[2]->startIncrementalQuery(query, varName);
}


void VqbInstancesForm::on_listBoxConditions_changed()
{
    //update query part
    m_queryPart = QString();
    foreach(QString triple, m_ui->listBoxConditions->items()) {
        m_queryPart.append(triple + " .\n");
    }
    emit queryChanged(VqbGlobal::addPrefixes(m_queryPart));

    updateCompleters();//BGP has changed
    updateVars();
}

void VqbInstancesForm::updateVars()
{
    //FIXME: add to comboboxes and completers
    //update m_varList;
    QRegExp rx(VqbGlobal::typeRegExp("var"));

    m_varList.clear();
    QString var;

    int pos = 0;
    while ((pos = rx.indexIn( m_queryPart, pos )) != -1) {
        var = rx.cap(1);
        if(!m_varList.contains(var) ) {
            m_varList.append(var);
        }
        pos += rx.matchedLength();
    }

    //add vars to list
    m_ui->listVars->clear();
    m_ui->listVars->addItems(m_varList);
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

    //FIXME:we should probably color everything. When a variable occurs twice, the results might be null because of that
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
    //query threads
    m_queryThreads.append(new QueryThread(this));
    m_queryThreads.append(new QueryThread(this));
    m_queryThreads.append(new QueryThread(this));

    //LineEdits
    m_ui->cbSubject->setLineEdit(new CompleterLineEdit(m_ui->cbSubject));
    m_ui->cbPredicate->setLineEdit(new CompleterLineEdit(m_ui->cbPredicate));
    m_ui->cbObject->setLineEdit(new CompleterLineEdit(m_ui->cbObject));

    connect(m_queryThreads[0], SIGNAL(resultFound(QString)), (CompleterLineEdit *) m_ui->cbSubject->lineEdit(), SLOT(addItem(QString)));
    connect(m_queryThreads[1], SIGNAL(resultFound(QString)), (CompleterLineEdit *) m_ui->cbPredicate->lineEdit(), SLOT(addItem(QString)));
    connect(m_queryThreads[2], SIGNAL(resultFound(QString)), (CompleterLineEdit *) m_ui->cbObject->lineEdit(), SLOT(addItem(QString)));

    //initializations    
    m_ui->cbSubject->lineEdit()->setText("?s");
    m_ui->cbPredicate->lineEdit()->setText("?p");
    m_ui->cbObject->lineEdit()->setText("?o");
    m_ui->cbRelation->insertItems(0, QStringList() << "equals" << "contains");

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
            this, SLOT(updateCurrentTriple()));

    //type updating signal
    connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateTypes()));
    connect(m_ui->checkBoxFilter, SIGNAL(toggled(bool)),
            this, SLOT(updateTypes()));

    //initial GUI population
    updateTypes();
    //updateCurrentTriple();
}


void VqbInstancesForm::on_buttonAddToOutput_clicked()
{
    if(m_ui->listVars->currentItem()) {
        emit(addVarToOutput(m_ui->listVars->currentItem()->text()));
    }
}



#include "vqbinstancesform.moc"



