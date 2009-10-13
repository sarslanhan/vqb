#include "vqbform.h"
#include "ui_vqbform.h"
#include "sparqlhighlighter.h"
#include "subjecttree.h"
#include "constraint.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include <QProcess>

#include <kdebug.h>
#include <kstandardaction.h>
#include <kaction.h>



class VqbForm::Private
{
public:
    Ui::VqbFormClass *ui;
    QPushButton *btnAdd;
    QVBoxLayout *topLayout; //layout holding the constraint lines
    QStringList queryTreeStrings;
    QList<SubjectTree*> queryTrees;
};

/*************      CONSTR, DESTR, INIT      ****************/

VqbForm::VqbForm(QWidget *parent)
        : QWidget(parent), d(new Private)
{
    d->ui = new Ui::VqbFormClass;
    d->ui->setupUi(this);
    init();
}

VqbForm::~VqbForm()
{
    delete d->ui;
}

void VqbForm::init()
{
    /* Layouts and visual elements */

    d->btnAdd = new QPushButton("New Query Tree");   // add button
    d->btnAdd->setBaseSize(100, 50);
    connect(d->btnAdd, SIGNAL(clicked()), this, SLOT(addQueryTree()));

    QHBoxLayout *qhbl = new QHBoxLayout; //button's layout
    qhbl->setDirection(QBoxLayout::RightToLeft);
    qhbl->addWidget(d->btnAdd, 1);
    qhbl->addStretch(10);

    QFrame* frame = new QFrame();
    QVBoxLayout* layout = new QVBoxLayout();
    frame->setLayout(layout);
    d->ui->scrollArea->setWidget(frame);

    d->topLayout = new QVBoxLayout;
    layout->addLayout(d->topLayout, 1);  //the top stack
    layout->addLayout(qhbl, 1);  //the bottom layout (holding the button)
    layout->addStretch(5);

    addQueryTree();
    new SparqlHighlighter(d->ui->queryViewer);

    /* Menus and initializations */

    QAction *refreshAction = KStandardAction::redisplay(this, SIGNAL(refresh()), this);
    refreshAction->setText(tr("&Refresh query"));
    refreshAction->setStatusTip(tr("Refreshes the query"));
    refreshAction->setShortcut(0);

    this->addAction(refreshAction);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(d->ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

/*************      PUBLIC SLOTS      ****************/

void VqbForm::queryTreeChanged(int index, QString queryTreeString)
{
    kDebug() << "Subject tree " << index << " has changed:" << queryTreeString;
    if (index < d->queryTreeStrings.count()) {
        d->queryTreeStrings[index] = queryTreeString;
    }
    refreshQuery();
}

void VqbForm::refreshQuery()
{
    QString query = "SELECT DISTINCT ";
    QString output = d->ui->outputList->toPlainText();
    output.replace("\n", " ");
    query.append(output);
    query.append(" \n WHERE { \n");
    foreach(QString s, d->queryTreeStrings) {
        query.append(s);
    }
    query.append("}\n");
    d->ui->queryViewer->setText(query);
}

void VqbForm::addVarToOutput(QString var)
{
    //kDebug() << "---- " << var;
    d->ui->outputList->appendPlainText(var);
    refreshQuery();
}

void VqbForm::tabChanged(int index)
{
    QString q;
    if (index == 1) {
        q = d->ui->queryViewer->toPlainText();
        q.replace("\n", " ");

        QString cmd = "sopranocmd --dbus org.kde.NepomukStorage --model main --querylang SPARQL query \"";
        cmd.append(q);

        QProcess script(this);
        script.start(cmd);

        if (!script.waitForStarted()) {
            qDebug() << "Query could not be started.";
            //return;
        }

        if (!script.waitForFinished()) {
            qDebug() << "Query could not be finished.";
            //return;
        }

        kDebug() << "Ran query: " << q;
        QString result = QString(script.readAllStandardOutput().data()) + (QString) script.readAllStandardError().data();
        d->ui->queryResults->setText(result);
    }
}

/*************        PRIVATE PARTS        ****************/
//not mine

void VqbForm::addQueryTree()
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

void VqbForm::queryTreeDeleted(int treeNumber)
{
    kDebug() << "Query Deleted " << treeNumber;
    int i;
    for(i=treeNumber; i<d->queryTrees.size()-1; i++) {
        kDebug() << "Moving " << i+1 << " to " << i;
        d->queryTrees[i] = d->queryTrees[i+1];
        d->queryTrees[i]->setTreeNumber(i);
        d->queryTreeStrings[i] = d->queryTreeStrings[i+1];
    }

    kDebug() << i << ", " << d->queryTrees.size();

    if (i < d->queryTrees.size()) {
        kDebug() << "Size = " << d->queryTrees.size();
        kDebug() << "Removing Query Tree " << i;
        d->queryTrees.removeAt(i);
        d->queryTreeStrings.removeAt(i);
        kDebug() << "Size = " << d->queryTrees.size();
    }
    refreshQuery();
}

#include "vqbform.moc"
