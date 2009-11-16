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
    VqbForm(parent),m_ui(new Ui::VqbInstancesForm),  m_queryThread(new QueryThread(this))
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

void VqbInstancesForm::on_listBoxConditions_changed()
{
    //update query part
    m_queryPart = QString();
    foreach(QString triple, m_ui->listBoxConditions->items()) {
        m_queryPart.append(triple + " .\n");
    }
    emit queryChanged(m_queryPart);

    //updateCompleters();//BGP has changed
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

void VqbInstancesForm::updateCompletersSubject(QString text)
{
    //FIXME: stop the previous query
    //m_ui->cbSubject->completionObject()->clear();

    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    m_queryThread->setQuery(constructCompletionQuery(text, 1),
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbSubject->lineEdit(), SLOT(addItem(QString)));

    m_queryThread->start();
}

void VqbInstancesForm::updateCompletersPredicate(QString text)
{
    Q_UNUSED(text);
}

void VqbInstancesForm::updateCompletersObject(QString text)
{
    Q_UNUSED(text);
}

QString VqbInstancesForm::constructCompletionQuery(QString text, int slotNumber)
{
    //initializing fields
    QString slotVar = "?slot";
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
    //building query
    QString query;
    switch(slotNumber) {
        case 1:
            query = slotVar + " " + predicate + " " + object + " . " + m_queryPart;
            break;
        case 2:
            query = subject + " " + slotVar + " " + object + " . " + m_queryPart;
            break;
        case 3:
            query = subject + " " + predicate + " " + slotVar + " . " + m_queryPart;
            break;
    }
    query += " FILTER regex( str(" + slotVar + "), '" + text + "', 'i') . ";

    //FIXME: use BGP

    return query;
}

/******** Utility functions *************/

void VqbInstancesForm::colorLineEdits()
{
    int hasResults = QueryThread::countQueryResults("SELECT * WHERE { " + m_currentTriple + " } LIMIT 1");
    //FIXME: use BGP

    QRegExp rx(VqbGlobal::typeRegExp("var"));

    //init the three LineEdits
    QLineEdit *lS = m_ui->cbSubject->lineEdit();
    QLineEdit *lP = m_ui->cbPredicate->lineEdit();
    QLineEdit *lO = m_ui->cbObject->lineEdit();

    //init the coloring Palettes
    QPalette palette( lO->palette() );//palette adjusted to given color
    palette.setColor( QPalette::Base, hasResults ? Qt::white : Qt::red );

    //perform the coloring
    lS->setPalette(palette);
    lP->setPalette(palette);
    lO->setPalette(palette);
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
    connect(m_ui->cbSubject->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbSubject, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbPredicate->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbPredicate, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbObject->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
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

    //updating completers
    connect(m_ui->cbSubject->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersSubject(QString)));

    //FIXME: activate colorLineEdits on editingFinished
    connect(m_ui->cbSubject->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEdits()));
    connect(m_ui->cbObject->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEdits()));
    connect(m_ui->cbPredicate->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEdits()));

    //initial GUI population
    updateTypes();
    updateCurrentTriple();
}


void VqbInstancesForm::on_buttonAddToOutput_clicked()
{
    if(m_ui->listVars->currentItem()) {
        emit(addVarToOutput(m_ui->listVars->currentItem()->text()));
    }
}



#include "vqbinstancesform.moc"

