#include "vqbform.h"
#include "ui_vqbform.h"
#include "constraint.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include <KDebug>
#include <KStandardAction>
#include <KAction>

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
    qhbl->addStretch( 5 );

    d->ui->verticalLayout->addLayout( d->topLayout, 0 );//the top stack
    d->ui->verticalLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)

    addConstraint( false, QString(), QString() );

    /* Menus and initializations */

    QAction *refreshAction = KStandardAction::redisplay( this, SLOT( slotRefresh() ), this );
    refreshAction->setText( tr("&Refresh query") );
    refreshAction->setStatusTip(tr("Refreshes the query"));
    refreshAction->setShortcut( 0 );

    this->addAction( refreshAction );
    this->setContextMenuPolicy( Qt::ActionsContextMenu );

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

        connect( c, SIGNAL(attachConstraint(int,QString)),
                 this, SLOT(attachConstraint(int,QString)) );
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
    QString query = "SELECT ";
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
}

 void VqbForm::addVarToOutput( QString var )
 {
     //kDebug() << "---- " << var;
     d->ui->outputList->appendPlainText( var + "\n");
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
