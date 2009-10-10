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
    QStringList constraintStrings;
    QStringList queryParts;
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

    d->btnAdd = new QPushButton("+");   // add button
    d->btnAdd->setBaseSize(100, 50);
    connect(d->btnAdd, SIGNAL(clicked()),
            this, SLOT(btnAdd_clicked()));

    d->topLayout = new QVBoxLayout;
    //d->topLayout->setSizeConstraint( QLayout::SetMaximumSize );

    QHBoxLayout *qhbl = new QHBoxLayout; //button's layout
    qhbl->setDirection(QBoxLayout::RightToLeft);
    qhbl->addWidget(d->btnAdd, 1);
    qhbl->addStretch(10);

    QFrame* frame = new QFrame();
    QVBoxLayout* layout = new QVBoxLayout();
    frame->setLayout(layout);
    d->ui->scrollArea->setWidget(frame);

    /*
        d->ui->verticalLayout->addLayout( d->topLayout, 1 );//the top stack
        d->ui->verticalLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
        d->ui->verticalLayout->addStretch( 5 );
    */

    layout->addLayout(d->topLayout, 1);  //the top stack
    layout->addLayout(qhbl, 1);  //the bottom layout (holding the button)
    layout->addStretch(5);

    addSubjectTree();

    new SparqlHighlighter(d->ui->queryViewer);

    /* Menus and initializations */

    QAction *refreshAction = KStandardAction::redisplay(this, SIGNAL(refresh()), this);
    refreshAction->setText(tr("&Refresh query"));
    refreshAction->setStatusTip(tr("Refreshes the query"));
    refreshAction->setShortcut(0);

    this->addAction(refreshAction);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(d->ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)));
}

/*************      PUBLIC SLOTS      ****************/
/*
void VqbForm::attachConstraint(int index, QString varName, QString varClass)
{
    //FIXME: correct or remove index
    Q_UNUSED(index);
    //addConstraint(true, varName, varClass);
    refreshQuery();
}
*/

void VqbForm::queryPartChanged(int index, QString queryPart)
{
    kDebug() << "Subject tree " << index << " has changed:" << queryPart;
    if (index < d->queryParts.count()) {
        d->queryParts[index] = queryPart;
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
    foreach(QString s, d->queryParts) {
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

void VqbForm::addSubjectTree()
{
    if (d->topLayout) {
        SubjectTree *st = new SubjectTree(d->queryParts.count(), this);
        d->topLayout->addWidget(st);
        d->queryParts.append("");

        //connect(this, SIGNAL(refresh()), st, SLOT(rebuildQueryPart()));

        connect(st, SIGNAL(queryPartChanged(int, QString)),
                this, SLOT(queryPartChanged(int, QString)));

        /*connect( c, SIGNAL(attachConstraint(int,QString,QString)),
                 this, SLOT(attachConstraint(int,QString,QString)) );
                 */
    }
}


void VqbForm::btnAdd_clicked()
{
    //addConstraint( false, QString(), QString() );
    addSubjectTree();
}


#include "vqbform.moc"
