#include "vqbschemaconstruct.h"
#include "vqbmainwindow.h"
#include "ui_vqbschemaconstruct.h"
#include "sparqlhighlighter.h"
#include "subjecttree.h"
#include "constraint.h"
#include "vqbglobal.h"

#include <QMainWindow>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRegExp>

#include <QProcess>

#include <kdebug.h>
#include <kstandardaction.h>
#include <kaction.h>
#include <KPushButton>
#include <KIcon>
#include <KIconLoader>
#include <KStandardGuiItem>


class VqbSchemaConstruct::Private
{
public:
    QPushButton *btnAdd;
    QVBoxLayout *topLayout;//holds the query trees
    QStringList queryTreeStrings;
    QList<SubjectTree*> queryTrees;
    QStringList outputs;
    QString query;
    Ui::VqbSchemaConstruct *ui;
};

/*************      CONSTR, DESTR, INIT      ****************/

VqbSchemaConstruct::VqbSchemaConstruct(VqbMainWindow *parent)
        : VqbForm(parent), d(new Private)
{
    d->ui = new Ui::VqbSchemaConstruct;
    d->ui->setupUi(this);
    init();
}

void VqbSchemaConstruct::init()
{
    /* Layouts and visual elements */

    d->btnAdd = new QPushButton("New Query Tree");   // add button
    d->btnAdd->setStatusTip("Adds a new query tree");
    d->btnAdd->setBaseSize(100, 50);
    connect(d->btnAdd, SIGNAL(clicked()), this, SLOT(addQueryTree()));

    QHBoxLayout *qhbl = new QHBoxLayout; //button's layout
    qhbl->setDirection(QBoxLayout::RightToLeft);
    qhbl->addWidget(d->btnAdd, 1);
    qhbl->addStretch(10);

    QVBoxLayout* layout = new QVBoxLayout();
    d->ui->scrollAreaWidgetContents->setLayout(layout);

    d->topLayout = new QVBoxLayout; //the top QueryTree stack
    layout->addLayout(d->topLayout, 1);
    layout->addLayout(qhbl, 1);  //the bottom layout (holding the button)
    layout->addStretch(5);

    addQueryTree();

    connect(d->ui->listSubject, SIGNAL(itemSelectionChanged()), this, SLOT(updateTriple()));
    connect(d->ui->listPredicate, SIGNAL(itemSelectionChanged()), this, SLOT(updateTriple()));
    connect(d->ui->listObject, SIGNAL(itemSelectionChanged()), this, SLOT(updateTriple()));
}


VqbSchemaConstruct::~VqbSchemaConstruct()
{
    delete d;
    delete d->ui;
}

/*************      PUBLIC SLOTS      ****************/

void VqbSchemaConstruct::queryTreeChanged(int index, QString queryTreeString)
{
    //kDebug() << "Subject tree " << index << " has changed:" << queryTreeString;
    if (index < d->queryTreeStrings.count()) {
        d->queryTreeStrings[index] = queryTreeString;
    }
    emitQueryChanged();
    populateOutputLists();
}


void VqbSchemaConstruct::queryTreeDeleted(int treeNumber)
{
    int i;
    for(i=treeNumber; i<d->queryTrees.size()-1; i++) {
        d->queryTrees[i] = d->queryTrees[i+1];
        d->queryTrees[i]->setTreeNumber(i);
        d->queryTreeStrings[i] = d->queryTreeStrings[i+1];
    }

    kDebug() << i << ", " << d->queryTrees.size();

    if (i < d->queryTrees.size()) {
        d->queryTrees.removeAt(i);
        d->queryTreeStrings.removeAt(i);
    }
    emitQueryChanged();
}

void VqbSchemaConstruct::addQueryTree()
{
    if (d->topLayout) {
        SubjectTree *qt = new SubjectTree(d->queryTrees.count(), this);
        d->topLayout->addWidget(qt);
        d->queryTrees.append(qt);
        d->queryTreeStrings.append("");

        connect(qt, SIGNAL(queryTreeChanged(int,QString)), this, SLOT(queryTreeChanged(int, QString)));
        connect(qt, SIGNAL(queryTreeDeleted(int)), this, SLOT(queryTreeDeleted(int)));
    }
}

void VqbSchemaConstruct::emitQueryChanged()
{
    QString query = "CONSTRUCT { ";
    foreach(QString s, d->ui->listBoxConditions->items()) {
        query.append(s + " ");
    }
    query.append(" } \n WHERE { \n");
    foreach(QString s, d->queryTreeStrings) {
        query.append(s);
    }
    query.append("}\n");

    d->query = query;

    emit queryChanged(query);
}

void VqbSchemaConstruct::populateOutputLists()
{
    kDebug() << d->query;
    d->ui->listObject->clear();
    d->ui->listSubject->clear();
    d->ui->listPredicate->clear();;

    //FIXME? use word boundaries?
    QRegExp rx(VqbGlobal::typeRegExp("var"));

    QStringList itemList;
    QString item;

    int pos = 0;
    while ((pos = rx.indexIn( d->query, pos )) != -1) {
        item = rx.cap(0);
        if(!itemList.contains(item) && !item.isEmpty()) {
            itemList.append(item);
        }
        pos += rx.matchedLength();
    }

    d->ui->listObject->addItems(itemList);
    d->ui->listSubject->addItems(itemList);
    d->ui->listPredicate->addItems(itemList);

    itemList.clear();

    rx.setPattern(VqbGlobal::typeRegExp("URI"));//non-literal URIs
    kDebug() << VqbGlobal::typeRegExp("URI");
    pos = 0;
    while ((pos = rx.indexIn( d->query, pos )) != -1) {
        item = rx.cap(0);
        if( !itemList.contains(item) && !item.isEmpty() ) {
            itemList.append(item);
        }
        pos += rx.matchedLength();
        kDebug() << "Captured: " << item;
    }

    d->ui->listObject->addItems(itemList);
    d->ui->listPredicate->addItems(itemList << "a"); //a (rdfs:type) predicate always needed

    itemList.clear();
    rx.setPattern(VqbGlobal::typeRegExp("Literal"));//literal URIs
    kDebug() << VqbGlobal::typeRegExp("Literal");
    pos = 0;
    while ((pos = rx.indexIn( d->query, pos )) != -1) {
        item = rx.cap(0);
        if(!itemList.contains(item) ) {
            itemList.append(item);
        }
        pos += rx.matchedLength();
        kDebug() << "Captured: " << item;
    }

    d->ui->listObject->addItems(itemList);
}

void VqbSchemaConstruct::updateTriple()
{
    QString triple = (d->ui->listSubject->currentItem() ? d->ui->listSubject->currentItem()->text() : QString()) + " " +
                     (d->ui->listPredicate->currentItem() ? d->ui->listPredicate->currentItem()->text() : QString()) + " " +
                     (d->ui->listObject->currentItem() ? d->ui->listObject->currentItem()->text() : QString());

    d->ui->listBoxConditions->lineEdit()->setText(triple);
}


#include "vqbschemaconstruct.moc"
