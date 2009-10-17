#include "vqbinstancesform.h"
#include "ui_vqbinstancesform.h"
#include "vqbmainwindow.h"
#include "vqbglobal.h"

#include </home/kde-devel/kde/include/kcombobox.h>
#include </home/kde-devel/kde/include/klineedit.h>
#include </home/kde-devel/kde/include/keditlistbox.h>
#include </home/kde-devel/kde/include/krandom.h>

#include <QRegExp>

VqbInstancesForm::VqbInstancesForm(VqbMainWindow *parent) :
    VqbForm(parent),
    m_ui(new Ui::VqbInstancesForm)
{
    m_ui->setupUi(this);
    init();
}

void VqbInstancesForm::init()
{
    m_ui->cbRelation->insertItems(0, QStringList() << "equals" << "contains");

    //connect signals

    connect(m_ui->cbSubject, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbPredicate, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbRelation, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->cbType, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateCurrentTriple()));
    connect(m_ui->checkBoxFilter, SIGNAL(stateChanged(int)),
            this, SLOT(updateCurrentTriple()));\

    connect(m_ui->cbObject, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateTypes()));

    //initializations

    m_ui->cbSubject->lineEdit()->setText("?s");
    m_ui->cbPredicate->lineEdit()->setText("?p");
    m_ui->cbObject->lineEdit()->setText("?o");

    updateCurrentTriple();
    updateTypes();
}

VqbInstancesForm::~VqbInstancesForm()
{
    delete m_ui;
}

void VqbInstancesForm::updateCurrentTriple()
{
    QString query;
    query = m_ui->cbSubject->currentText() + " " + m_ui->cbPredicate->currentText() + " ";
    if(m_ui->checkBoxFilter->isChecked()) {//Filter ON
        QString var = QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;
        QString relation = m_ui->cbRelation->currentText();
        if (relation == "equals") {

            query.append( var + " . FILTER regex(" + var + ", '^" + m_ui->cbObject->currentText() + "$', 'i') . ");
        } else if (relation == "contains") {
            query.append( var + " . FILTER regex(" + var + ", '" + m_ui->cbObject->currentText() + "', 'i') . ") ;
        }
    }
    else {//no filter
        //FIXME: handles types
        query.append(VqbGlobal::getCanonicalForm(m_ui->cbObject->currentText(), m_ui->cbType->currentText()));
    }

    m_ui->listBoxConditions->lineEdit()->setText(query);
}

void VqbInstancesForm::updateTypes()
{
    QString text = m_ui->cbObject->currentText();
    m_ui->cbType->clear();
    QStringList types = VqbGlobal::literalTypes();

    if(!text.isEmpty()) {
        QRegExp rx;
        foreach(QString type, types) {
            rx.setPattern(VqbGlobal::typeRegExp(type));
            if ( !rx.exactMatch( text ) ) {
                //kDebug() << "Removing:";
                types.removeAt( types.indexOf( type ) );
            }
        }
    }
    m_ui->cbType->clear();
    m_ui->cbType->insertItems(-1, types);

}

#include "vqbinstancesform.moc"
