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
    QTextEdit *queryViewer;
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
               this, SLOT( on_btnAdd_clicked() ) );

    d->topLayout = new QVBoxLayout;
    //d->topLayout->setSizeConstraint( QLayout::SetMaximumSize );

    QHBoxLayout *qhbl = new QHBoxLayout; //button's layout
    qhbl->setDirection( QBoxLayout::RightToLeft );
    qhbl->addWidget( d->btnAdd, 1 );
    qhbl->addStretch( 5 );

    d->queryViewer = new QTextEdit;
    //d->queryViewer->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint( QLayout::SetMaximumSize );
    mainLayout->addLayout( d->topLayout, 0 );//the top stack
    mainLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
    mainLayout->addStretch( 1 );
    mainLayout->addWidget( d->queryViewer, 0, Qt::AlignBottom );//the top stack

    this->setLayout( mainLayout );

    addConstraintLine();

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

void VqbForm::addConstraintLine()
{
    //QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>( this->layout() );
    if( d->topLayout ) {
        Constraint *c = new Constraint( d->constraintStrings.count(), this );
        d->topLayout->addWidget( c  );
        d->constraintStrings.append("");

        connect( this, SIGNAL(refresh()),
                 c, SLOT(returnConstraint()) );
        connect( c, SIGNAL( constraintChanged(int,QString) ),
                 this, SLOT( constraintChanged(int,QString) ));
    }
}

void VqbForm::constraintChanged( int index, QString constraint )
{
    kDebug() << "Constraint " << index << " has changed:" << constraint;
    if( index < d->constraintStrings.count() ) {
        d->constraintStrings[index] = constraint;
    }

    QString query = "WHERE { ";
    foreach( QString s, d->constraintStrings ) {
        query.append( s + "\n" );
    }

    query.append( "}\n" );
    d->queryViewer->setText( query );
}

void VqbForm::on_btnAdd_clicked()
{
    addConstraintLine();
}

void VqbForm::slotRefresh()
{
    emit refresh();
}

#include "vqbform.moc"
