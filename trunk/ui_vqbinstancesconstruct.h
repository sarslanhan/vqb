/********************************************************************************
** Form generated from reading UI file 'vqbinstancesconstruct.ui'
**
** Created: Sun Dec 13 14:10:18 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VQBINSTANCESCONSTRUCT_H
#define UI_VQBINSTANCESCONSTRUCT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kcombobox.h"
#include "keditlistbox.h"

QT_BEGIN_NAMESPACE

class Ui_VqbInstancesConstruct
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxOutputs;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBoxOTriples;
    QGridLayout *gridLayout_2;
    QLabel *labelSOutputs;
    KComboBox *cbSubjectOutputs;
    QLabel *labelPOutputs;
    KComboBox *cbPredicateOutputs;
    QLabel *labelOOutputs;
    KComboBox *cbObjectOutputs;
    QLabel *label_2;
    KComboBox *cbTypeOutputs;
    KEditListBox *listBoxOutputs;
    QGroupBox *groupBoxConditions;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBoxCTriples;
    QGridLayout *gridLayout;
    QLabel *labelSConditions;
    KComboBox *cbSubjectConditions;
    QLabel *labelPConditions;
    KComboBox *cbPredicateConditions;
    QCheckBox *checkBoxFilterConditions;
    KComboBox *cbRelationConditions;
    QLabel *labelOConditions;
    KComboBox *cbObjectConditions;
    KComboBox *cbTypeConditions;
    QLabel *label;
    KEditListBox *listBoxConditions;

    void setupUi(QWidget *VqbInstancesConstruct)
    {
        if (VqbInstancesConstruct->objectName().isEmpty())
            VqbInstancesConstruct->setObjectName(QString::fromUtf8("VqbInstancesConstruct"));
        VqbInstancesConstruct->resize(720, 480);
        verticalLayout = new QVBoxLayout(VqbInstancesConstruct);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBoxOutputs = new QGroupBox(VqbInstancesConstruct);
        groupBoxOutputs->setObjectName(QString::fromUtf8("groupBoxOutputs"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBoxOutputs->sizePolicy().hasHeightForWidth());
        groupBoxOutputs->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(groupBoxOutputs);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBoxOTriples = new QGroupBox(groupBoxOutputs);
        groupBoxOTriples->setObjectName(QString::fromUtf8("groupBoxOTriples"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBoxOTriples->sizePolicy().hasHeightForWidth());
        groupBoxOTriples->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(9);
        groupBoxOTriples->setFont(font);
        groupBoxOTriples->setAutoFillBackground(true);
        gridLayout_2 = new QGridLayout(groupBoxOTriples);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelSOutputs = new QLabel(groupBoxOTriples);
        labelSOutputs->setObjectName(QString::fromUtf8("labelSOutputs"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelSOutputs->sizePolicy().hasHeightForWidth());
        labelSOutputs->setSizePolicy(sizePolicy2);
        labelSOutputs->setLayoutDirection(Qt::RightToLeft);

        gridLayout_2->addWidget(labelSOutputs, 0, 0, 1, 1);

        cbSubjectOutputs = new KComboBox(groupBoxOTriples);
        cbSubjectOutputs->setObjectName(QString::fromUtf8("cbSubjectOutputs"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(cbSubjectOutputs->sizePolicy().hasHeightForWidth());
        cbSubjectOutputs->setSizePolicy(sizePolicy3);
        cbSubjectOutputs->setEditable(true);

        gridLayout_2->addWidget(cbSubjectOutputs, 0, 1, 1, 2);

        labelPOutputs = new QLabel(groupBoxOTriples);
        labelPOutputs->setObjectName(QString::fromUtf8("labelPOutputs"));
        labelPOutputs->setLayoutDirection(Qt::RightToLeft);

        gridLayout_2->addWidget(labelPOutputs, 1, 0, 1, 1);

        cbPredicateOutputs = new KComboBox(groupBoxOTriples);
        cbPredicateOutputs->setObjectName(QString::fromUtf8("cbPredicateOutputs"));
        cbPredicateOutputs->setEditable(true);

        gridLayout_2->addWidget(cbPredicateOutputs, 1, 1, 1, 2);

        labelOOutputs = new QLabel(groupBoxOTriples);
        labelOOutputs->setObjectName(QString::fromUtf8("labelOOutputs"));
        labelOOutputs->setLayoutDirection(Qt::RightToLeft);

        gridLayout_2->addWidget(labelOOutputs, 2, 0, 1, 1);

        cbObjectOutputs = new KComboBox(groupBoxOTriples);
        cbObjectOutputs->setObjectName(QString::fromUtf8("cbObjectOutputs"));
        cbObjectOutputs->setEditable(true);

        gridLayout_2->addWidget(cbObjectOutputs, 2, 1, 1, 2);

        label_2 = new QLabel(groupBoxOTriples);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_2, 3, 1, 1, 1);

        cbTypeOutputs = new KComboBox(groupBoxOTriples);
        cbTypeOutputs->setObjectName(QString::fromUtf8("cbTypeOutputs"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(cbTypeOutputs->sizePolicy().hasHeightForWidth());
        cbTypeOutputs->setSizePolicy(sizePolicy4);

        gridLayout_2->addWidget(cbTypeOutputs, 3, 2, 1, 1);


        horizontalLayout->addWidget(groupBoxOTriples);

        listBoxOutputs = new KEditListBox(groupBoxOutputs);
        listBoxOutputs->setObjectName(QString::fromUtf8("listBoxOutputs"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(3);
        sizePolicy5.setVerticalStretch(1);
        sizePolicy5.setHeightForWidth(listBoxOutputs->sizePolicy().hasHeightForWidth());
        listBoxOutputs->setSizePolicy(sizePolicy5);
        listBoxOutputs->setFont(font);
        listBoxOutputs->setFlat(false);

        horizontalLayout->addWidget(listBoxOutputs);


        verticalLayout->addWidget(groupBoxOutputs);

        groupBoxConditions = new QGroupBox(VqbInstancesConstruct);
        groupBoxConditions->setObjectName(QString::fromUtf8("groupBoxConditions"));
        sizePolicy.setHeightForWidth(groupBoxConditions->sizePolicy().hasHeightForWidth());
        groupBoxConditions->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(groupBoxConditions);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBoxCTriples = new QGroupBox(groupBoxConditions);
        groupBoxCTriples->setObjectName(QString::fromUtf8("groupBoxCTriples"));
        sizePolicy1.setHeightForWidth(groupBoxCTriples->sizePolicy().hasHeightForWidth());
        groupBoxCTriples->setSizePolicy(sizePolicy1);
        groupBoxCTriples->setFont(font);
        groupBoxCTriples->setAutoFillBackground(true);
        gridLayout = new QGridLayout(groupBoxCTriples);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelSConditions = new QLabel(groupBoxCTriples);
        labelSConditions->setObjectName(QString::fromUtf8("labelSConditions"));
        labelSConditions->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(labelSConditions, 0, 0, 1, 1);

        cbSubjectConditions = new KComboBox(groupBoxCTriples);
        cbSubjectConditions->setObjectName(QString::fromUtf8("cbSubjectConditions"));
        cbSubjectConditions->setEditable(true);

        gridLayout->addWidget(cbSubjectConditions, 0, 1, 1, 2);

        labelPConditions = new QLabel(groupBoxCTriples);
        labelPConditions->setObjectName(QString::fromUtf8("labelPConditions"));
        labelPConditions->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(labelPConditions, 1, 0, 1, 1);

        cbPredicateConditions = new KComboBox(groupBoxCTriples);
        cbPredicateConditions->setObjectName(QString::fromUtf8("cbPredicateConditions"));
        cbPredicateConditions->setEditable(true);

        gridLayout->addWidget(cbPredicateConditions, 1, 1, 1, 2);

        checkBoxFilterConditions = new QCheckBox(groupBoxCTriples);
        checkBoxFilterConditions->setObjectName(QString::fromUtf8("checkBoxFilterConditions"));

        gridLayout->addWidget(checkBoxFilterConditions, 2, 1, 1, 1);

        cbRelationConditions = new KComboBox(groupBoxCTriples);
        cbRelationConditions->setObjectName(QString::fromUtf8("cbRelationConditions"));

        gridLayout->addWidget(cbRelationConditions, 2, 2, 1, 1);

        labelOConditions = new QLabel(groupBoxCTriples);
        labelOConditions->setObjectName(QString::fromUtf8("labelOConditions"));
        labelOConditions->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(labelOConditions, 3, 0, 1, 1);

        cbObjectConditions = new KComboBox(groupBoxCTriples);
        cbObjectConditions->setObjectName(QString::fromUtf8("cbObjectConditions"));
        cbObjectConditions->setEditable(true);

        gridLayout->addWidget(cbObjectConditions, 3, 1, 1, 2);

        cbTypeConditions = new KComboBox(groupBoxCTriples);
        cbTypeConditions->setObjectName(QString::fromUtf8("cbTypeConditions"));

        gridLayout->addWidget(cbTypeConditions, 4, 2, 1, 1);

        label = new QLabel(groupBoxCTriples);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 4, 1, 1, 1);


        horizontalLayout_2->addWidget(groupBoxCTriples);

        listBoxConditions = new KEditListBox(groupBoxConditions);
        listBoxConditions->setObjectName(QString::fromUtf8("listBoxConditions"));
        sizePolicy5.setHeightForWidth(listBoxConditions->sizePolicy().hasHeightForWidth());
        listBoxConditions->setSizePolicy(sizePolicy5);
        listBoxConditions->setFont(font);
        listBoxConditions->setFlat(false);

        horizontalLayout_2->addWidget(listBoxConditions);


        verticalLayout->addWidget(groupBoxConditions);

        QWidget::setTabOrder(cbSubjectOutputs, cbPredicateOutputs);
        QWidget::setTabOrder(cbPredicateOutputs, cbObjectOutputs);
        QWidget::setTabOrder(cbObjectOutputs, cbTypeOutputs);
        QWidget::setTabOrder(cbTypeOutputs, cbSubjectConditions);
        QWidget::setTabOrder(cbSubjectConditions, cbPredicateConditions);
        QWidget::setTabOrder(cbPredicateConditions, checkBoxFilterConditions);
        QWidget::setTabOrder(checkBoxFilterConditions, cbRelationConditions);
        QWidget::setTabOrder(cbRelationConditions, cbObjectConditions);
        QWidget::setTabOrder(cbObjectConditions, cbTypeConditions);

        retranslateUi(VqbInstancesConstruct);

        QMetaObject::connectSlotsByName(VqbInstancesConstruct);
    } // setupUi

    void retranslateUi(QWidget *VqbInstancesConstruct)
    {
        VqbInstancesConstruct->setWindowTitle(QApplication::translate("VqbInstancesConstruct", "Form", 0, QApplication::UnicodeUTF8));
        groupBoxOutputs->setTitle(QApplication::translate("VqbInstancesConstruct", "Outputs", 0, QApplication::UnicodeUTF8));
        groupBoxOTriples->setTitle(QApplication::translate("VqbInstancesConstruct", "Triple", 0, QApplication::UnicodeUTF8));
        labelSOutputs->setText(QApplication::translate("VqbInstancesConstruct", "Subject", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbSubjectOutputs->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Subject Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelPOutputs->setText(QApplication::translate("VqbInstancesConstruct", "Predicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbPredicateOutputs->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Predicate Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelOOutputs->setText(QApplication::translate("VqbInstancesConstruct", "Object", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbObjectOutputs->setToolTip(QApplication::translate("VqbInstancesConstruct", "Enter a string, integer, date or URI.\n"
"\n"
"Examples:\n"
"	foaf:Person\n"
"	5\n"
"	Knud Moeller\n"
"            	<http://purl.org/dc/terms/title>\n"
"	", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        cbObjectOutputs->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Object field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_2->setText(QApplication::translate("VqbInstancesConstruct", "Type", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbTypeOutputs->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Object (Literal) Type", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        listBoxOutputs->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Output Triples", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        listBoxOutputs->setTitle(QApplication::translate("VqbInstancesConstruct", "Graph Pattern", 0, QApplication::UnicodeUTF8));
        groupBoxConditions->setTitle(QApplication::translate("VqbInstancesConstruct", "Conditions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBoxCTriples->setToolTip(QApplication::translate("VqbInstancesConstruct", "Enter a string, integer, date or URI.\n"
"\n"
"Examples:\n"
"	foaf:Person\n"
"	5\n"
"	Knud Moeller\n"
"            	<http://purl.org/dc/terms/title>\n"
"	", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBoxCTriples->setTitle(QApplication::translate("VqbInstancesConstruct", "Triple", 0, QApplication::UnicodeUTF8));
        labelSConditions->setText(QApplication::translate("VqbInstancesConstruct", "Subject", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbSubjectConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Subject Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelPConditions->setText(QApplication::translate("VqbInstancesConstruct", "Predicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbPredicateConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Predicate Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        checkBoxFilterConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Filter on Object Value", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        checkBoxFilterConditions->setText(QApplication::translate("VqbInstancesConstruct", "Filter", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbRelationConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Filter Relation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelOConditions->setText(QApplication::translate("VqbInstancesConstruct", "Object", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbObjectConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Object Field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        cbTypeConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Object (Literal) Type", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("VqbInstancesConstruct", "Type", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        listBoxConditions->setStatusTip(QApplication::translate("VqbInstancesConstruct", "Conditional Triples", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        listBoxConditions->setTitle(QApplication::translate("VqbInstancesConstruct", "Graph Pattern", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VqbInstancesConstruct: public Ui_VqbInstancesConstruct {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VQBINSTANCESCONSTRUCT_H
