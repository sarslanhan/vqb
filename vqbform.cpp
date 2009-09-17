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

class VqbForm::Private
{
public:
    Ui::VqbFormClass *ui;
    QPushButton *btnAdd;
    QVBoxLayout *topLayout; //layout holding the constraint lines
    QList<Constraint*> constraints;
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
    d->btnAdd = new QPushButton( "+" );
    d->btnAdd->setBaseSize(100, 50);
    connect( d->btnAdd, SIGNAL( clicked() ),
               this, SLOT( on_btnAdd_clicked() ) );

    d->topLayout = new QVBoxLayout;
    d->topLayout->setSizeConstraint( QLayout::SetMaximumSize );

    QHBoxLayout *qhbl = new QHBoxLayout;
    qhbl->setDirection( QBoxLayout::RightToLeft );
    qhbl->addWidget( d->btnAdd, 1 );
    qhbl->addStretch( 5 );

    d->queryViewer = new QTextEdit;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout( d->topLayout, 0 );//the top stack
    mainLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
    mainLayout->addWidget( d->queryViewer, 0 );//the top stack
    mainLayout->addStretch( 1 );
    


    this->setLayout( mainLayout );

    addConstraintLine();
}

VqbForm::~VqbForm()
{
    delete d->ui;
}

void VqbForm::addConstraintLine()
{
    //QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>( this->layout() );
    if( d->topLayout ) {
        Constraint *c = new Constraint( d->constraints.count(), this );
        d->constraints.append( c );
        d->topLayout->addWidget( c  );
    }
}

void VqbForm::on_btnAdd_clicked()
{
    addConstraintLine();
}
