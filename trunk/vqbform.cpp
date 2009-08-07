#include "vqbform.h"
#include "ui_vqbform.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>


VqbForm::VqbForm(QWidget *parent)
    : QWidget(parent), ui(new Ui::VqbFormClass)
{
    ui->setupUi(this);
    init();
    addConstraintLine();
}

void VqbForm::init()
{
    m_btnAdd = new QPushButton( "+" );
    m_btnAdd->setBaseSize(100, 50);
    connect( m_btnAdd, SIGNAL( clicked() ),
               this, SLOT( on_btnAdd_clicked() ) );

    m_topLayout = new QVBoxLayout;

    QHBoxLayout *qhbl = new QHBoxLayout;
    qhbl->setDirection( QBoxLayout::RightToLeft );
    qhbl->addWidget( m_btnAdd, 1 );
    qhbl->addStretch( 5 );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout( m_topLayout, 0 );//the top stack
    mainLayout->addLayout( qhbl, 1 );//the bottom layout (holding the button)
    mainLayout->addStretch( 1 );
    this->setLayout( mainLayout );
}

VqbForm::~VqbForm()
{
    delete ui;
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
    if( m_topLayout ) {
        m_topLayout->addLayout( createConstraintLine() );
    }
}

void VqbForm::on_btnAdd_clicked()
{
    addConstraintLine();
}
