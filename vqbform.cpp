#include "vqbform.h"
#include "ui_vqbform.h"
#include "vqbbackend.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

#include <KDebug>

class VqbForm::Private
{
public:
    Ui::VqbFormClass *ui;
    QPushButton *btnAdd;
    QVBoxLayout *topLayout; //layout holding the constraint lines
    VqbBackend *backend;
};

VqbForm::VqbForm(QWidget *parent)
    : QWidget(parent), d(new Private)
{
    d->ui = new Ui::VqbFormClass;
    d->ui->setupUi(this);
    init();
    addConstraintLine();
}

void VqbForm::init()
{
    d->btnAdd = new QPushButton( "+" );
    d->btnAdd->setBaseSize(100, 50);
    connect( d->btnAdd, SIGNAL( clicked() ),
               this, SLOT( on_btnAdd_clicked() ) );

    d->topLayout = new QVBoxLayout;

    QHBoxLayout *qhbl = new QHBoxLayout;
    qhbl->setDirection( QBoxLayout::RightToLeft );
    qhbl->addWidget( d->btnAdd, 1 );
    qhbl->addStretch( 5 );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout( d->topLayout, 0 );//the top stack
    mainLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
    mainLayout->addStretch( 1 );
    this->setLayout( mainLayout );

    d->backend = new VqbBackend( this );
    d->backend->findSubjectsWithLabels();//finds and feeds it to the GUI
}

VqbForm::~VqbForm()
{
    delete d->ui;
}

QHBoxLayout *VqbForm::createConstraintLine()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QComboBox *cb1 = new QComboBox();
    QComboBox *cb2 = new QComboBox();
    QComboBox *cb3 = new QComboBox();
    QLineEdit *le = new QLineEdit();

    layout->addWidget( cb1, 1 );
    layout->addWidget( cb2, 1 );
    layout->addWidget( cb3, 1 );
    layout->addWidget( le, 1 );

    return layout;
}

void VqbForm::addConstraintLine()
{
    //QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>( this->layout() );
    if( d->topLayout ) {
        d->topLayout->addLayout( createConstraintLine() );
    }
}

void VqbForm::addSubjects( QStringList subjects )
{
    kDebug() << "*** Received them: " << subjects;
}

void VqbForm::on_btnAdd_clicked()
{
    addConstraintLine();
}
