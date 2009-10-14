#include "vqbinstancesform.h"
#include "ui_vqbinstancesform.h"

VqbInstancesForm::VqbInstancesForm(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::VqbInstancesForm)
{
    m_ui->setupUi(this);
}

VqbInstancesForm::~VqbInstancesForm()
{
    delete m_ui;
}

