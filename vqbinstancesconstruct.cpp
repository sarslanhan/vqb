#include "vqbinstancesconstruct.h"
#include "ui_vqbinstancesconstruct.h"
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

VqbInstancesConstruct::VqbInstancesConstruct(VqbMainWindow *parent) :
    VqbForm(parent),m_ui(new Ui::VqbInstancesConstruct),  m_queryThread(new QueryThread(this))
{
    m_ui->setupUi(this);
    init();
    connect(this, SIGNAL(errorMessage(QString)), parent, SLOT(postErrorMessage(QString)));
}


VqbInstancesConstruct::~VqbInstancesConstruct()
{
    delete m_ui;
}

/******* Slots for GUI events **************/

void VqbInstancesConstruct::updateCurrentTripleC()
{
    QString triple;
    triple = m_ui->cbSubjectConditions->currentText() + " " + m_ui->cbPredicateConditions->currentText() + " ";
    triple += VqbGlobal::constructObject(m_ui->checkBoxFilterConditions->isChecked(),
                                         m_ui->cbRelationConditions->currentText(),
                                         m_ui->cbObjectConditions->currentText(),
                                         m_ui->cbTypeConditions->currentText());


    m_ui->listBoxConditions->lineEdit()->setText(triple);
    m_currentTriple = triple;
}

void VqbInstancesConstruct::updateCurrentTripleO()
{
    QString triple;
    triple = m_ui->cbSubjectOutputs->currentText() + " " + m_ui->cbPredicateOutputs->currentText() + " ";
    triple += VqbGlobal::constructObject(false,
                                         QString(),
                                         m_ui->cbObjectOutputs->currentText(),
                                         m_ui->cbTypeOutputs->currentText());


    m_ui->listBoxOutputs->lineEdit()->setText(triple);
    m_currentTriple = triple;
}


void VqbInstancesConstruct::updateTypesC()
{
    if(m_ui->checkBoxFilterConditions->isChecked()) {
        m_ui->cbTypeConditions->setEnabled(false);
        return;
    }

    m_ui->cbTypeConditions->setEnabled(true);
    QString text = m_ui->cbObjectConditions->currentText();
    m_ui->cbTypeConditions->clear();
    QStringList types = VqbGlobal::objectTypes();

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
    m_ui->cbTypeConditions->clear();
    m_ui->cbTypeConditions->insertItems(-1, types);
}

void VqbInstancesConstruct::updateTypesO()
{
    QString text = m_ui->cbObjectOutputs->currentText();
    m_ui->cbTypeOutputs->clear();
    QStringList types = VqbGlobal::objectTypes();

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
    m_ui->cbTypeOutputs->clear();
    m_ui->cbTypeOutputs->insertItems(-1, types);
}

void VqbInstancesConstruct::updateVars()
{
    //FIXME?: add to completers
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

    //kDebug() << m_varList << m_queryPart;
    //add vars to combo boxes
    m_ui->cbSubjectOutputs->clear();
    m_ui->cbPredicateOutputs->clear();
    m_ui->cbObjectOutputs->clear();
    m_ui->cbSubjectOutputs->addItems(m_varList);
    m_ui->cbPredicateOutputs->addItems(m_varList);
    m_ui->cbObjectOutputs->addItems(m_varList);

    m_ui->cbSubjectConditions->clear();
    m_ui->cbPredicateConditions->clear();
    m_ui->cbObjectConditions->clear();
    m_ui->cbSubjectConditions->addItems(m_varList);
    m_ui->cbPredicateConditions->addItems(m_varList);
    m_ui->cbObjectConditions->addItems(m_varList);

    //FIXME: remove nonexistent vars from output list
}

void VqbInstancesConstruct::on_listBoxConditions_changed()
{
    emitQueryChanged();
    updateVars();
}


void VqbInstancesConstruct::on_listBoxOutputs_changed()
{
    emitQueryChanged();
    updateVars();
}

void VqbInstancesConstruct::emitQueryChanged()
{
    //update query part
    QString m_queryPart = "CONSTRUCT { ";
    foreach(QString triple, m_ui->listBoxOutputs->items()) {
        m_queryPart.append(triple + " ");
    }
    m_queryPart.append(" } \n WHERE { \n");
    foreach(QString triple, m_ui->listBoxConditions->items()) {
        m_queryPart.append(triple + " .\n");
    }
    m_queryPart.append("}\n");

    emit queryChanged(m_queryPart);
}

void VqbInstancesConstruct::updateCompletersSubjectC(QString text)
{
    //FIXME: stop the previous query
    //m_ui->cbSubject->completionObject()->clear();

    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    m_queryThread->setQuery(constructCompletionQuery(text, 1, true),
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbSubjectConditions->lineEdit(), SLOT(addItem(QString)));

    m_queryThread->start();
}

void VqbInstancesConstruct::updateCompletersPredicateC(QString text)
{
    //FIXME: stop the previous query
    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    QString query = constructCompletionQuery(text, 2, true);
    m_queryThread->setQuery(query,
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbPredicateConditions->lineEdit(), SLOT(addItem(QString)));
    //kDebug() << query;
    m_queryThread->start();
}

void VqbInstancesConstruct::updateCompletersObjectC(QString text)
{
    //FIXME: stop the previous query
    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    QString query = constructCompletionQuery(text, 3, true);
    m_queryThread->setQuery(query,
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbObjectConditions->lineEdit(), SLOT(addItem(QString)));
    //kDebug() << query;

    m_queryThread->start();
}


void VqbInstancesConstruct::updateCompletersSubjectO(QString text)
{
    //FIXME: stop the previous query

    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    m_queryThread->setQuery(constructCompletionQuery(text, 1, false),
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbSubjectOutputs->lineEdit(), SLOT(addItem(QString)));

    m_queryThread->start();
}

void VqbInstancesConstruct::updateCompletersPredicateO(QString text)
{
    //FIXME: stop the previous query
    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    QString query = constructCompletionQuery(text, 2, false);
    m_queryThread->setQuery(query,
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbPredicateOutputs->lineEdit(), SLOT(addItem(QString)));
    m_queryThread->start();
}

void VqbInstancesConstruct::updateCompletersObjectO(QString text)
{
    //FIXME: stop the previous query
    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    QString query = constructCompletionQuery(text, 3, false);
    m_queryThread->setQuery(query,
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbObjectOutputs->lineEdit(), SLOT(addItem(QString)));

    m_queryThread->start();
}

QString VqbInstancesConstruct::constructCompletionQuery(QString text, int slotNumber, bool isWherePart)
{
    //initializing fields
    QString slotVar = "?slot";
    QString query;

    if(isWherePart) {
        QString subject = m_ui->cbSubjectConditions->currentText();
        subject = subject.isEmpty() ? VqbGlobal::randomVarName() : subject;
        QString predicate = m_ui->cbPredicateConditions->currentText();
        predicate = predicate.isEmpty() ? VqbGlobal::randomVarName() : predicate;
        QString object = m_ui->cbObjectConditions->currentText();
        object = object.isEmpty() ? VqbGlobal::randomVarName() :
                                    VqbGlobal::constructObject(m_ui->checkBoxFilterConditions->isChecked(),
                                                               m_ui->cbRelationConditions->currentText(),
                                                               m_ui->cbObjectConditions->currentText(),
                                                               m_ui->cbTypeConditions->currentText());
        //building query
        switch(slotNumber) {
            case 1:
                query = slotVar + " " + predicate + " " + object + " . ";
                break;
            case 2:
                query = subject + " " + slotVar + " " + object + " . ";
                break;
            case 3:
                query = subject + " " + predicate + " " + slotVar + " . ";
                break;
        }
        query += " FILTER regex( str(" + slotVar + "), '" + text + "', 'i') . ";

        //basic graph pattern (BGP - triples added so far)
        foreach(QString triple, m_ui->listBoxConditions->items()) {
            query.append(triple + " . ");
        }
    }
    else {
        //FIXME: proper CONSTRUCT autocompletion
        QString subject = m_ui->cbSubjectOutputs->currentText();
        subject = subject.isEmpty() ? VqbGlobal::randomVarName() : subject;
        QString predicate = m_ui->cbPredicateOutputs->currentText();
        predicate = predicate.isEmpty() ? VqbGlobal::randomVarName() : predicate;
        QString object = m_ui->cbObjectOutputs->currentText();
        object = object.isEmpty() ? VqbGlobal::randomVarName() :
                                    VqbGlobal::constructObject(false,
                                                               QString(),
                                                               m_ui->cbObjectOutputs->currentText(),
                                                               m_ui->cbTypeOutputs->currentText());
        //building query
        switch(slotNumber) {
            case 1:
                query = predicate +  " rdfs:domain " + slotVar + " . " + predicate + " rdfs:range " + object + " . ";
                break;
            case 2:
                query = slotVar +  " rdfs:domain " + subject + " . " + slotVar + " rdfs:range " + object + " . ";
                break;
            case 3:
                query = predicate +  " rdfs:domain " + subject + " . " + predicate + " rdfs:range " + slotVar + " . ";
                break;
        }
        query += " FILTER regex( str(" + slotVar + "), '" + text + "', 'i') . ";

        //No BGP for outputs
    }    

    return query;
}

/******** Utility functions *************/

void VqbInstancesConstruct::colorLineEditsC()
{
    //FIXME: display an informative message about the error (something like "0 results found");
    int hasResults = QueryThread::countQueryResults("SELECT * WHERE { " + m_currentTriple + " } LIMIT 1");
    //FIXME: use BGP

    QRegExp rx(VqbGlobal::typeRegExp("var"));

    //init the three LineEdits
    QLineEdit *lS = m_ui->cbSubjectConditions->lineEdit();
    QLineEdit *lP = m_ui->cbPredicateConditions->lineEdit();
    QLineEdit *lO = m_ui->cbObjectConditions->lineEdit();

    //init the coloring Palettes
    //init the coloring Palettes
    QPalette palette( lO->palette() );//palette adjusted to given color
    if(hasResults) {
        palette.setColor( QPalette::Base, Qt::white );
    }
    else  {
        palette.setColor( QPalette::Base, Qt::red );
        emit errorMessage("No results found!");
    }


    //perform the coloring
    lS->setPalette(palette);
    lP->setPalette(palette);
    lO->setPalette(palette);
}

/********** Initialization *************/


void VqbInstancesConstruct::init()
{
    //query threads
    m_queryThreads.append(new QueryThread(this));
    m_queryThreads.append(new QueryThread(this));
    m_queryThreads.append(new QueryThread(this));

    //LineEdits
    m_ui->cbSubjectConditions->setLineEdit(new CompleterLineEdit());
    m_ui->cbPredicateConditions->setLineEdit(new CompleterLineEdit());
    m_ui->cbObjectConditions->setLineEdit(new CompleterLineEdit());
    m_ui->cbSubjectOutputs->setLineEdit(new CompleterLineEdit());
    m_ui->cbPredicateOutputs->setLineEdit(new CompleterLineEdit());
    m_ui->cbObjectOutputs->setLineEdit(new CompleterLineEdit());

    connect(m_queryThreads[0], SIGNAL(resultFound(QString)), (CompleterLineEdit *) m_ui->cbSubjectConditions->lineEdit(), SLOT(addItem(QString)));
    connect(m_queryThreads[1], SIGNAL(resultFound(QString)), (CompleterLineEdit *) m_ui->cbPredicateConditions->lineEdit(), SLOT(addItem(QString)));
    connect(m_queryThreads[2], SIGNAL(resultFound(QString)), (CompleterLineEdit *) m_ui->cbObjectConditions->lineEdit(), SLOT(addItem(QString)));

    //triple initializations
    m_ui->cbSubjectConditions->lineEdit()->setText("?s");
    m_ui->cbPredicateConditions->lineEdit()->setText("?p");
    m_ui->cbObjectConditions->lineEdit()->setText("?o");
    m_ui->cbRelationConditions->insertItems(0, QStringList() << "equals" << "contains");
    m_ui->cbSubjectOutputs->lineEdit()->setText("?s");
    m_ui->cbPredicateOutputs->lineEdit()->setText("?p");
    m_ui->cbObjectOutputs->lineEdit()->setText("?o");

    //triple updating signals /Conditions
    connect(m_ui->cbSubjectConditions->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbSubjectConditions, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbPredicateConditions->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbPredicateConditions, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbObjectConditions->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbObjectConditions, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbRelationConditions, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->cbTypeConditions, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleC()));
    connect(m_ui->checkBoxFilterConditions, SIGNAL(stateChanged(int)),
            this, SLOT(updateCurrentTripleC()));

    //triple updating signals /Outputs
    connect(m_ui->cbSubjectOutputs->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTripleO()));
    connect(m_ui->cbSubjectOutputs, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleO()));
    connect(m_ui->cbPredicateOutputs->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTripleO()));
    connect(m_ui->cbPredicateOutputs, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleO()));
    connect(m_ui->cbObjectOutputs->lineEdit(), SIGNAL(textChanged(QString)),//(editingFinished()),
            this, SLOT(updateCurrentTripleO()));
    connect(m_ui->cbObjectOutputs, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleO()));
    connect(m_ui->cbTypeOutputs, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTripleO()));

    //type updating signal /Conditions
    connect(m_ui->cbObjectConditions, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateTypesC()));
    connect(m_ui->checkBoxFilterConditions, SIGNAL(toggled(bool)),
            this, SLOT(updateTypesC()));

    //type updating signal /Outputs
    connect(m_ui->cbObjectOutputs, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateTypesO()));

    //updating completers /Conditions
    connect(m_ui->cbSubjectConditions->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersSubjectC(QString)));
    connect(m_ui->cbObjectConditions->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersObjectC(QString)));
    connect(m_ui->cbPredicateConditions->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersPredicateC(QString)));

    //updating completers /Outputs
    connect(m_ui->cbSubjectOutputs->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersSubjectO(QString)));
    connect(m_ui->cbObjectOutputs->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersObjectO(QString)));
    connect(m_ui->cbPredicateOutputs->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersPredicateO(QString)));

    //color LineEdits /Conditions
    connect(m_ui->cbSubjectConditions->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEditsC()));
    connect(m_ui->cbObjectConditions->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEditsC()));
    connect(m_ui->cbPredicateConditions->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEditsC()));

    //initial GUI population
    updateTypesC();
    updateTypesO();
    updateCurrentTripleC();
    updateCurrentTripleO();

}



#include "vqbinstancesconstruct.moc"



