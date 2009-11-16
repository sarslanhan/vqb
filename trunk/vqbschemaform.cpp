#include "vqbschemaform.h"
#include "vqbmainwindow.h"
#include "ui_vqbform.h"
#include "sparqlhighlighter.h"
#include "subjecttree.h"
#include "constraint.h"

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

#include <QProcess>

#include <kdebug.h>
#include <kstandardaction.h>
#include <kaction.h>
#include <KPushButton>
#include <KIcon>
#include <KIconLoader>
#include <KStandardGuiItem>


class VqbSchemaForm::Private
{
public:
    QPushButton *btnAdd;
    QVBoxLayout *topLayout;//holds the query trees
    QStringList queryTreeStrings;
    QList<SubjectTree*> queryTrees;
};

/*************      CONSTR, DESTR, INIT      ****************/

VqbSchemaForm::VqbSchemaForm(VqbMainWindow *parent)
        : VqbForm(parent), d(new Private)
{
    init();
}

void VqbSchemaForm::init()
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
    setLayout(layout);

    d->topLayout = new QVBoxLayout; //the top QueryTree stack
    layout->addLayout(d->topLayout, 1);
    layout->addLayout(qhbl, 1);  //the bottom layout (holding the button)
    layout->addStretch(5);

    addQueryTree();
}


VqbSchemaForm::~VqbSchemaForm()
{
    delete d;
}

/*************      PUBLIC SLOTS      ****************/

void VqbSchemaForm::queryTreeChanged(int index, QString queryTreeString)
{
    //kDebug() << "Subject tree " << index << " has changed:" << queryTreeString;
    if (index < d->queryTreeStrings.count()) {
        d->queryTreeStrings[index] = queryTreeString;
    }
    emitQueryChanged();
}


void VqbSchemaForm::queryTreeDeleted(int treeNumber)
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

void VqbSchemaForm::addQueryTree()
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

void VqbSchemaForm::emitQueryChanged()
{
    QString query;
    foreach(QString s, d->queryTreeStrings) {
        query.append(s);
    }
    emit queryChanged(query);
}

#include "vqbschemaform.moc"
