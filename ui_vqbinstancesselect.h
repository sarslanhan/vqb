/********************************************************************************
** Form generated from reading UI file 'vqbinstancesselect.ui'
**
** Created: Sun Dec 13 14:10:18 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VQBINSTANCESSELECT_H
#define UI_VQBINSTANCESSELECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>
#include "kcombobox.h"
#include "keditlistbox.h"
#include "kpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_VqbInstancesSelect
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *labelS;
    KComboBox *cbSubject;
    QLabel *labelP;
    KComboBox *cbPredicate;
    QCheckBox *checkBoxFilter;
    KComboBox *cbRelation;
    QLabel *labelO;
    KComboBox *cbObject;
    KComboBox *cbType;
    QLabel *label_2;
    KEditListBox *listBoxConditions;
    QGroupBox *groupBox1;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QLabel *labelOutputVars;
    KPushButton *buttonRemoveFromOutput;
    KPushButton *buttonUp;
    KPushButton *buttonDown;
    QListWidget *listOutputs;
    QListWidget *listVars;
    KPushButton *buttonAddToOutput;

    void setupUi(QWidget *VqbInstancesSelect)
    {
        if (VqbInstancesSelect->objectName().isEmpty())
            VqbInstancesSelect->setObjectName(QString::fromUtf8("VqbInstancesSelect"));
        VqbInstancesSelect->resize(720, 480);
        gridLayout = new QGridLayout(VqbInstancesSelect);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(VqbInstancesSelect);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(9);
        groupBox->setFont(font);
        groupBox->setAutoFillBackground(true);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelS = new QLabel(groupBox);
        labelS->setObjectName(QString::fromUtf8("labelS"));
        labelS->setLayoutDirection(Qt::RightToLeft);

        gridLayout_2->addWidget(labelS, 0, 0, 1, 1);

        cbSubject = new KComboBox(groupBox);
        cbSubject->setObjectName(QString::fromUtf8("cbSubject"));
        cbSubject->setEditable(true);

        gridLayout_2->addWidget(cbSubject, 0, 1, 1, 2);

        labelP = new QLabel(groupBox);
        labelP->setObjectName(QString::fromUtf8("labelP"));
        labelP->setLayoutDirection(Qt::RightToLeft);

        gridLayout_2->addWidget(labelP, 1, 0, 1, 1);

        cbPredicate = new KComboBox(groupBox);
        cbPredicate->setObjectName(QString::fromUtf8("cbPredicate"));
        cbPredicate->setEditable(true);

        gridLayout_2->addWidget(cbPredicate, 1, 1, 1, 2);

        checkBoxFilter = new QCheckBox(groupBox);
        checkBoxFilter->setObjectName(QString::fromUtf8("checkBoxFilter"));

        gridLayout_2->addWidget(checkBoxFilter, 2, 1, 1, 1);

        cbRelation = new KComboBox(groupBox);
        cbRelation->setObjectName(QString::fromUtf8("cbRelation"));

        gridLayout_2->addWidget(cbRelation, 2, 2, 1, 1);

        labelO = new QLabel(groupBox);
        labelO->setObjectName(QString::fromUtf8("labelO"));
        labelO->setLayoutDirection(Qt::RightToLeft);

        gridLayout_2->addWidget(labelO, 3, 0, 1, 1);

        cbObject = new KComboBox(groupBox);
        cbObject->setObjectName(QString::fromUtf8("cbObject"));
        cbObject->setEditable(true);

        gridLayout_2->addWidget(cbObject, 3, 1, 1, 2);

        cbType = new KComboBox(groupBox);
        cbType->setObjectName(QString::fromUtf8("cbType"));

        gridLayout_2->addWidget(cbType, 4, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_2, 4, 1, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        listBoxConditions = new KEditListBox(VqbInstancesSelect);
        listBoxConditions->setObjectName(QString::fromUtf8("listBoxConditions"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(3);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(listBoxConditions->sizePolicy().hasHeightForWidth());
        listBoxConditions->setSizePolicy(sizePolicy1);
        listBoxConditions->setFont(font);
        listBoxConditions->setFlat(false);

        gridLayout->addWidget(listBoxConditions, 0, 1, 1, 1);

        groupBox1 = new QGroupBox(VqbInstancesSelect);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        sizePolicy1.setHeightForWidth(groupBox1->sizePolicy().hasHeightForWidth());
        groupBox1->setSizePolicy(sizePolicy1);
        groupBox1->setFont(font);
        gridLayout_3 = new QGridLayout(groupBox1);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(groupBox1);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        labelOutputVars = new QLabel(groupBox1);
        labelOutputVars->setObjectName(QString::fromUtf8("labelOutputVars"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(labelOutputVars->sizePolicy().hasHeightForWidth());
        labelOutputVars->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(labelOutputVars, 0, 2, 1, 2);

        buttonRemoveFromOutput = new KPushButton(groupBox1);
        buttonRemoveFromOutput->setObjectName(QString::fromUtf8("buttonRemoveFromOutput"));
        buttonRemoveFromOutput->setFont(font);

        gridLayout_3->addWidget(buttonRemoveFromOutput, 3, 1, 1, 1);

        buttonUp = new KPushButton(groupBox1);
        buttonUp->setObjectName(QString::fromUtf8("buttonUp"));

        gridLayout_3->addWidget(buttonUp, 4, 2, 1, 1);

        buttonDown = new KPushButton(groupBox1);
        buttonDown->setObjectName(QString::fromUtf8("buttonDown"));

        gridLayout_3->addWidget(buttonDown, 4, 3, 1, 1);

        listOutputs = new QListWidget(groupBox1);
        listOutputs->setObjectName(QString::fromUtf8("listOutputs"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(3);
        sizePolicy4.setHeightForWidth(listOutputs->sizePolicy().hasHeightForWidth());
        listOutputs->setSizePolicy(sizePolicy4);
        listOutputs->setFont(font);
        listOutputs->setDragDropMode(QAbstractItemView::InternalMove);
        listOutputs->setMovement(QListView::Free);

        gridLayout_3->addWidget(listOutputs, 1, 2, 3, 2);

        listVars = new QListWidget(groupBox1);
        listVars->setObjectName(QString::fromUtf8("listVars"));
        sizePolicy4.setHeightForWidth(listVars->sizePolicy().hasHeightForWidth());
        listVars->setSizePolicy(sizePolicy4);
        listVars->setDragDropMode(QAbstractItemView::InternalMove);
        listVars->setMovement(QListView::Free);

        gridLayout_3->addWidget(listVars, 1, 0, 4, 1);

        buttonAddToOutput = new KPushButton(groupBox1);
        buttonAddToOutput->setObjectName(QString::fromUtf8("buttonAddToOutput"));
        buttonAddToOutput->setFont(font);

        gridLayout_3->addWidget(buttonAddToOutput, 2, 1, 1, 1);


        gridLayout->addWidget(groupBox1, 1, 1, 1, 1);

#ifndef QT_NO_SHORTCUT
        labelS->setBuddy(cbSubject);
        labelP->setBuddy(cbPredicate);
        labelO->setBuddy(cbObject);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(cbSubject, cbPredicate);
        QWidget::setTabOrder(cbPredicate, checkBoxFilter);
        QWidget::setTabOrder(checkBoxFilter, cbRelation);
        QWidget::setTabOrder(cbRelation, cbObject);
        QWidget::setTabOrder(cbObject, cbType);
        QWidget::setTabOrder(cbType, listVars);
        QWidget::setTabOrder(listVars, buttonRemoveFromOutput);
        QWidget::setTabOrder(buttonRemoveFromOutput, listOutputs);
        QWidget::setTabOrder(listOutputs, buttonUp);
        QWidget::setTabOrder(buttonUp, buttonDown);

        retranslateUi(VqbInstancesSelect);

        QMetaObject::connectSlotsByName(VqbInstancesSelect);
    } // setupUi

    void retranslateUi(QWidget *VqbInstancesSelect)
    {
        VqbInstancesSelect->setWindowTitle(QApplication::translate("VqbInstancesSelect", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("VqbInstancesSelect", "Triple", 0, QApplication::UnicodeUTF8));
        labelS->setText(QApplication::translate("VqbInstancesSelect", "Subject", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbSubject->setStatusTip(QApplication::translate("VqbInstancesSelect", "Subject Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelP->setText(QApplication::translate("VqbInstancesSelect", "Predicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbPredicate->setStatusTip(QApplication::translate("VqbInstancesSelect", "Predicate Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        checkBoxFilter->setStatusTip(QApplication::translate("VqbInstancesSelect", "Filter on Object Value", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        checkBoxFilter->setText(QApplication::translate("VqbInstancesSelect", "Filter", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbRelation->setStatusTip(QApplication::translate("VqbInstancesSelect", "Filter Relation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelO->setText(QApplication::translate("VqbInstancesSelect", "Object", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbObject->setToolTip(QApplication::translate("VqbInstancesSelect", "Enter a string, integer, date or URI.\n"
"\n"
"Examples:\n"
"	foaf:Person\n"
"	5\n"
"	Knud Moeller\n"
"            	<http://purl.org/dc/terms/title>\n"
"	", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        cbObject->setStatusTip(QApplication::translate("VqbInstancesSelect", "Object field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        cbType->setStatusTip(QApplication::translate("VqbInstancesSelect", "Object (Literal) Type", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_2->setText(QApplication::translate("VqbInstancesSelect", "Type", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listBoxConditions->setToolTip(QApplication::translate("VqbInstancesSelect", "Conditional Triples", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        listBoxConditions->setStatusTip(QApplication::translate("VqbInstancesSelect", "Conditional Triples", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        listBoxConditions->setTitle(QApplication::translate("VqbInstancesSelect", "Graph Pattern", 0, QApplication::UnicodeUTF8));
        groupBox1->setTitle(QApplication::translate("VqbInstancesSelect", "Output Variables", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VqbInstancesSelect", "Available variables:", 0, QApplication::UnicodeUTF8));
        labelOutputVars->setText(QApplication::translate("VqbInstancesSelect", "Output variables:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonRemoveFromOutput->setToolTip(QApplication::translate("VqbInstancesSelect", "Remove from Output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonRemoveFromOutput->setStatusTip(QApplication::translate("VqbInstancesSelect", "Removes Variable from Output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buttonRemoveFromOutput->setText(QApplication::translate("VqbInstancesSelect", "<- Remove", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonUp->setToolTip(QApplication::translate("VqbInstancesSelect", "Move Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonUp->setStatusTip(QApplication::translate("VqbInstancesSelect", "Moves variable up one position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        buttonDown->setToolTip(QApplication::translate("VqbInstancesSelect", "Move Down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonDown->setStatusTip(QApplication::translate("VqbInstancesSelect", "Moves variable down one position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        buttonAddToOutput->setToolTip(QApplication::translate("VqbInstancesSelect", "Add to Output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonAddToOutput->setStatusTip(QApplication::translate("VqbInstancesSelect", "Adds Variable to Output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buttonAddToOutput->setText(QApplication::translate("VqbInstancesSelect", "Add ->", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VqbInstancesSelect: public Ui_VqbInstancesSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VQBINSTANCESSELECT_H
