#include "vqbinstancesform.h"
#include "ui_vqbinstancesform.h"
#include "vqbmainwindow.h"
#include "vqbglobal.h"
#include "querythread.h"

#include </home/kde-devel/kde/include/kcombobox.h>
#include </home/kde-devel/kde/include/klineedit.h>
#include </home/kde-devel/kde/include/keditlistbox.h>
#include </home/kde-devel/kde/include/krandom.h>
#include </home/kde-devel/kde/include/kdebug.h>

#include <QRegExp>
#include <QTimer>

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
    if(m_ui->checkBoxFilter->isChecked()) {//Filter ON
        QString var = QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;
        QString relation = m_ui->cbRelation->currentText();
        if (relation == "contains") {
            triple.append( var + " . FILTER regex(" + var + ", '" + m_ui->cbObject->currentText() + "', 'i') . ") ;
        }
        else if (relation == "equals") {
            triple.append( var + " . FILTER regex(" + var + ", '^" + m_ui->cbObject->currentText() + "$', 'i') . ");
        }
    }
    else {//Filter OFF
        triple.append(VqbGlobal::getCanonicalForm(m_ui->cbObject->currentText(), m_ui->cbType->currentText()));
    }

    m_ui->listBoxConditions->lineEdit()->setText(triple);
    m_currentTriple = triple;

    //update completers for comboboxes
    updateCompleters();
}

void VqbInstancesForm::updateTypes()
{
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

    colorLineEdits(QueryThread::countQueryResults(query));

    /*
    if( QueryThread::countQueryResults(query) == 0 ) {
        //setColor( lineEditOS, comboBoxOP->lineEdit(), lineEditOO, comboBoxTypesO->currentText(), Qt::red );

    }
    else {
        colorLineEdits(Qt::white);
        //setColor( lineEditOS, comboBoxOP->lineEdit(), lineEditOO, comboBoxTypesO->currentText(), Qt::white );
    }
    */

/*    //generate random name
    srand( QTime::currentTime().msec() );
    QString varName;
    varName.setNum( 100 + rand() % 1000 );
    varName.prepend("xyz" + varName);

    //query for SUBJECT autocompletion
    query = "?" + varName + " " + comboBoxOP->currentText() + " " + VisualQueryBuilderConsts::getCanonicalForm( lineEditOO->text(), comboBoxTypesO->currentText() );
    setCompletersOS( qTh->runQuery( query, varName ) );

    //query for OBJECT
    query = lineEditOS->text() + " " + comboBoxOP->currentText() + " " + "?" + varName;
    setCompletersOO( qTh->runQuery( query, varName ) );

    //query for PREDICATE
    setCompletersOP( QStringList() );
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
    if (!(rx.exactMatch( lO->text() ) && m_ui->cbType->currentText() == "var")) { //only non-variables are colored
        lO->setPalette(paletteColored);
    } else { //color variables with white
        lO->setPalette(paletteWhite);
    }
}

/********** Initialization *************/


void VqbInstancesForm::init()
{
    m_ui->cbRelation->insertItems(0, QStringList() << "equals" << "contains");

    //triple updating signals
    connect(m_ui->cbSubject, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbSubject, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbPredicate, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbPredicate, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
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

    //initializations
    m_ui->cbSubject->lineEdit()->setText("?s");
    m_ui->cbPredicate->lineEdit()->setText("?p");
    m_ui->cbObject->lineEdit()->setText("?o");

    //initial GUI population
    updateCurrentTriple();
    updateTypes();
}


#include "vqbinstancesform.moc"
