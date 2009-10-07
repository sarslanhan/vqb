#include "vqbform.h"
#include "ui_vqbform.h"
#include "constraint.h"
#include "sparqlhighlighter.h"
#include "subjecttree.h"

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
};

VqbForm::VqbForm(QWidget *parent)
    : QWidget(parent), d(new Private)
{
    d->ui = new Ui::VqbFormClass;
    d->ui->setupUi(this);
    init();
}

void VqbForm::init()
{
    /* Layouts and visual elements */

    d->btnAdd = new QPushButton( "+" ); // add button
    d->btnAdd->setBaseSize(100, 50);
    connect( d->btnAdd, SIGNAL( clicked() ),
               this, SLOT( btnAdd_clicked() ) );

    d->topLayout = new QVBoxLayout;
    //d->topLayout->setSizeConstraint( QLayout::SetMaximumSize );

    QHBoxLayout *qhbl = new QHBoxLayout; //button's layout
    qhbl->setDirection( QBoxLayout::RightToLeft );
    qhbl->addWidget( d->btnAdd, 1 );
    qhbl->addStretch( 10 );

    QFrame* frame = new QFrame();
    QVBoxLayout* layout = new QVBoxLayout();
    frame->setLayout(layout);
    d->ui->scrollArea->setWidget(frame);

/*
    d->ui->verticalLayout->addLayout( d->topLayout, 1 );//the top stack
    d->ui->verticalLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
    d->ui->verticalLayout->addStretch( 5 );
*/

    layout->addLayout( d->topLayout, 1 );//the top stack
    layout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
    layout->addStretch( 5 );

    addConstraint( false, QString(), QString() );

     SparqlHighlighter *highlighter = new SparqlHighlighter(d->ui->queryViewer);

    /* Menus and initializations */

    QAction *refreshAction = KStandardAction::redisplay( this, SLOT( slotRefresh() ), this );
    refreshAction->setText( tr("&Refresh query") );
    refreshAction->setStatusTip(tr("Refreshes the query"));
    refreshAction->setShortcut( 0 );

    this->addAction( refreshAction );
    this->setContextMenuPolicy( Qt::ActionsContextMenu );

    connect( d->ui->tabWidget, SIGNAL(currentChanged(int)),
             this, SLOT(tabChanged(int)) );
}

VqbForm::~VqbForm()
{
    delete d->ui;
}

void VqbForm::addConstraint( bool isAttached, QString parentVarName, QString parentClass )
{
    //QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>( this->layout() );
    if( d->topLayout ) {
        Constraint *c = new Constraint( d->constraintStrings.count(), this, isAttached, parentVarName, parentClass );
        d->topLayout->addWidget( c  );
        d->constraintStrings.append("");

        connect( this, SIGNAL(refresh()),
                 c, SLOT(returnConstraint()) );

        connect( c, SIGNAL( constraintChanged(int,QString) ),
                 this, SLOT( constraintChanged(int,QString) ));

        /*connect( c, SIGNAL(attachConstraint(int,QString,QString)),
                 this, SLOT(attachConstraint(int,QString,QString)) );
                 */
    }
}

void VqbForm::constraintChanged( int index, QString constraint )
{
    kDebug() << "Constraint " << index << " has changed:" << constraint;
    if( index < d->constraintStrings.count() ) {
        d->constraintStrings[index] = constraint;
    }
    refreshQuery();
}

void VqbForm::refreshQuery()
{
    QString query = "SELECT DISTINCT ";
    QString output = d->ui->outputList->toPlainText();
    output.replace( "\n", " " );
    query.append( output );
    query.append( " \n WHERE { \n");
    foreach( QString s, d->constraintStrings ) {
        query.append( s + "\n" );
    }
    query.append( "}\n" );
    d->ui->queryViewer->setText( query );
}

void VqbForm::attachConstraint( int index, QString varName, QString varClass )
{
    //FIXME: correct or remove index
    Q_UNUSED( index );
    addConstraint( true, varName, varClass );
    refreshQuery();
}

 void VqbForm::addVarToOutput( QString var )
 {
     //kDebug() << "---- " << var;
     d->ui->outputList->appendPlainText( var );
     refreshQuery();
 }

 void VqbForm::tabChanged(int index)
 {
    QString q;
    if( index == 1 ) {
        q = d->ui->queryViewer->toPlainText();
        q.replace( "\n", " " );

        QString cmd = "sopranocmd --dbus org.kde.NepomukStorage --model main --querylang SPARQL query \"";
        cmd.append( q );

        QProcess script(this);
        script.start( cmd );

        if (!script.waitForStarted()) {
             qDebug() << "Query could not be started.";
             //return;
        }

        if (!script.waitForFinished()){
             qDebug() << "Query could not be finished.";
             //return;
        }

    kDebug() << "Ran query: " << q;
    QString result = QString(script.readAllStandardOutput().data()) + (QString) script.readAllStandardError().data();
    d->ui->queryResults->setText( result );


    }

 }

void VqbForm::btnAdd_clicked()
{
    addConstraint( false, QString(), QString() );
}

void VqbForm::slotRefresh()
{
    emit refresh();
}

#include "vqbform.moc"
