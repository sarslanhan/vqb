#include "vqbinstancesselect.h"
#include "ui_vqbinstancesselect.h"
#include "vqbmainwindow.h"
#include "vqbglobal.h"
#include "querythread.h"
#include "completerlineedit.h"

#include <kcombobox.h>
#include <klineedit.h>
#include <keditlistbox.h>
#include <krandom.h>
#include <kdebug.h>
#include <kcompletion.h>

#include <QRegExp>
#include <QTimer>
#include <QCompleter>
#include <QModelIndex>

VqbInstancesSelect::VqbInstancesSelect(VqbMainWindow *parent) :
    VqbForm(parent),m_ui(new Ui::VqbInstancesSelect),  m_queryThread(new QueryThread(this))
{
    m_ui->setupUi(this);
    init();

    connect(this, SIGNAL(errorMessage(QString)), parent, SLOT(postErrorMessage(QString)));
    //FIXME: input hints
}

VqbInstancesSelect::~VqbInstancesSelect()
{
    delete m_ui;
}

/******* Slots for GUI events **************/

void VqbInstancesSelect::updateCurrentTriple()
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

void VqbInstancesSelect::updateTypes()
{
    if(m_ui->checkBoxFilter->isChecked()) {
        m_ui->cbType->setEnabled(false);
        return;
    }

    m_ui->cbType->setEnabled(true);
    QString text = m_ui->cbObject->currentText();
    m_ui->cbType->clear();
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
    m_ui->cbType->clear();
    m_ui->cbType->insertItems(-1, types);
}

void VqbInstancesSelect::updateQuery()
{
    //update query part
    QString query = "SELECT DISTINCT ";
    for(int i=0; i<m_ui->listOutputs->count(); i++) {
        query.append(m_ui->listOutputs->item(i)->text() + " ");
    }
    query.append(" \n WHERE { \n");
    foreach(QString triple, m_ui->listBoxConditions->items()) {
        query.append(triple + " .\n");
    }
    query.append("}\n");

    m_queryPart = query;
    emit queryChanged(m_queryPart);
}

void VqbInstancesSelect::updateVars()
{
    //FIXME: add to completers
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
    m_ui->cbSubject->clear();
    m_ui->cbPredicate->clear();
    m_ui->cbObject->clear();
    m_ui->cbSubject->addItems(m_varList);
    m_ui->cbPredicate->addItems(m_varList);
    m_ui->cbObject->addItems(m_varList);

    m_ui->cbSubject->lineEdit()->setText("?s");
    m_ui->cbPredicate->lineEdit()->setText("?p");
    m_ui->checkBoxFilter->setChecked(false);
    m_ui->cbObject->lineEdit()->setText("?o");

    //FIXME: remove nonexistent vars from output list
}

void VqbInstancesSelect::updateCompletersSubject(QString text)
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

void VqbInstancesSelect::updateCompletersPredicate(QString text)
{
    //FIXME: stop the previous query
    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    QString query = constructCompletionQuery(text, 2);
    m_queryThread->setQuery(query,
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbPredicate->lineEdit(), SLOT(addItem(QString)));
    //kDebug() << query;
    m_queryThread->start();
}

void VqbInstancesSelect::updateCompletersObject(QString text)
{
    //FIXME: stop the previous query
    if(text.isEmpty()) { //don't complete empty string
        return;
    }

    QString query = constructCompletionQuery(text, 3);
    m_queryThread->setQuery(query,
                            "?slot", QueryThread::IncrementalQuery);
    disconnect(m_queryThread, SIGNAL(resultFound(QString)), 0, 0);
    connect(m_queryThread, SIGNAL(resultFound(QString)),
            (CompleterLineEdit*)m_ui->cbObject->lineEdit(), SLOT(addItem(QString)));
    //kDebug() << query;

    m_queryThread->start();
}

QString VqbInstancesSelect::constructCompletionQuery(QString text, int slotNumber)
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

    foreach(QString triple, m_ui->listBoxConditions->items()) {
        query.append(triple + " . ");
        //kDebug() << triple;
    }

    //kDebug() << "Returning: " << query;
    return query;
}

/******** Utility functions *************/

void VqbInstancesSelect::colorLineEdits()
{
    int hasResults = QueryThread::countQueryResults("SELECT * WHERE { " + m_currentTriple + " } LIMIT 1");

    QPalette palette( m_ui->cbSubject->lineEdit()->palette() );//palette adjusted to given color
    if(hasResults) {
        palette.setColor( QPalette::Base, Qt::white );
        palette.setColor( QPalette::Window, Qt::white );
    }
    else  {
        palette.setColor( QPalette::Base, Qt::red );
        palette.setColor( QPalette::Window, Qt::red );
        emit errorMessage("No results found!");
    }

    //perform the coloring
    m_ui->cbSubject->lineEdit()->setPalette(palette);
    m_ui->cbPredicate->lineEdit()->setPalette(palette);
    m_ui->cbObject->lineEdit()->setPalette(palette);
}

/********** Initialization *************/


void VqbInstancesSelect::init()
{
    m_ui->buttonUp->setIcon(KIcon("arrow-up"));
    m_ui->buttonDown->setIcon(KIcon("arrow-down"));

    //query threads
    //FIXME: remove query threads (manage them properly)
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
    connect(m_ui->cbObject->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersObject(QString)));
    connect(m_ui->cbPredicate->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(updateCompletersPredicate(QString)));

    connect(m_ui->cbSubject->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEdits()));
    connect(m_ui->cbObject->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEdits()));
    connect(m_ui->cbPredicate->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(colorLineEdits()));

    //initial GUI population
    updateTypes();
    updateCurrentTriple();

    //FIXME: populate comboboxes with all possible corresponding values
}

/******************* GUI component events ****************/
void VqbInstancesSelect::on_buttonAddToOutput_clicked()
{
    if(m_ui->listVars->currentItem()) {
        m_ui->listOutputs->addItem(m_ui->listVars->currentItem()->text());
    }
    updateQuery();
}

void VqbInstancesSelect::on_listVars_doubleClicked(QModelIndex index)
{
    Q_UNUSED(index);
    if(m_ui->listVars->currentItem()) {
        m_ui->listOutputs->addItem(m_ui->listVars->currentItem()->text());
    }
    updateQuery();
}


void VqbInstancesSelect::on_buttonRemoveFromOutput_clicked()
{
    m_ui->listOutputs->takeItem(m_ui->listOutputs->currentRow());
    updateQuery();
}

void VqbInstancesSelect::on_listBoxConditions_changed()
{
    kDebug() << "Conditions changeeeeeeeeeeeeeeeeeeeeeeeeed";

    m_ui->cbSubject->blockSignals(true);
    m_ui->cbPredicate->blockSignals(true);
    m_ui->cbObject->blockSignals(true);
    m_ui->cbSubject->lineEdit()->blockSignals(true);
    m_ui->cbPredicate->lineEdit()->blockSignals(true);
    m_ui->cbObject->lineEdit()->blockSignals(true);
    updateQuery();
    updateVars();
    m_ui->cbSubject->blockSignals(false);
    m_ui->cbPredicate->blockSignals(false);
    m_ui->cbObject->blockSignals(false);
    m_ui->cbSubject->lineEdit()->blockSignals(false);
    m_ui->cbPredicate->lineEdit()->blockSignals(false);
    m_ui->cbObject->lineEdit()->blockSignals(false);
}

void VqbInstancesSelect::on_listBoxConditions_added(QString text)
{
    Q_UNUSED(text);
    updateQuery();
    updateVars();
}

void VqbInstancesSelect::on_listBoxConditions_removed(QString text)
{
    on_listBoxConditions_added(text);
}



void VqbInstancesSelect::on_buttonUp_clicked()
{
    QListWidget* lw = m_ui->listOutputs;
    int i = lw->currentRow();
    if(i > 0) {
        QListWidgetItem *item = lw->takeItem(i);
        lw->insertItem(i-1, item);
        lw->setCurrentRow(i-1);
    }
}


void VqbInstancesSelect::on_buttonDown_clicked()
{
    QListWidget* lw = m_ui->listOutputs;
    int i = lw->currentRow();
    if(i < lw->count()-1) {
        QListWidgetItem *item = lw->takeItem(i);
        lw->insertItem(i+1, item);
        lw->setCurrentRow(i+1);
    }
}


#include "vqbinstancesselect.moc"


