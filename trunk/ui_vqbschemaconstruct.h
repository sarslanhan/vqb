/********************************************************************************
** Form generated from reading UI file 'vqbschemaconstruct.ui'
**
** Created: Sun Dec 13 14:10:18 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VQBSCHEMACONSTRUCT_H
#define UI_VQBSCHEMACONSTRUCT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "keditlistbox.h"
#include "klineedit.h"
#include "klistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_VqbSchemaConstruct
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBoxConditions;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGroupBox *groupBoxOutput;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBoxSubject;
    QVBoxLayout *verticalLayout_3;
    KLineEdit *lineEditFilterSubject;
    KListWidget *listSubject;
    QCheckBox *cbShowAllSubject;
    QGroupBox *groupBoxPredicate;
    QVBoxLayout *verticalLayout_4;
    KLineEdit *lineEditFilterPredicate;
    KListWidget *listPredicate;
    QCheckBox *cbShowAllPredicate;
    QGroupBox *groupBoxObject;
    QVBoxLayout *verticalLayout_5;
    KLineEdit *lineEditFilterObject;
    KListWidget *listObject;
    QCheckBox *cbShowAllObject;
    KEditListBox *listBoxConditions;

    void setupUi(QWidget *VqbSchemaConstruct)
    {
        if (VqbSchemaConstruct->objectName().isEmpty())
            VqbSchemaConstruct->setObjectName(QString::fromUtf8("VqbSchemaConstruct"));
        VqbSchemaConstruct->resize(720, 480);
        verticalLayout_2 = new QVBoxLayout(VqbSchemaConstruct);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBoxConditions = new QGroupBox(VqbSchemaConstruct);
        groupBoxConditions->setObjectName(QString::fromUtf8("groupBoxConditions"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBoxConditions->sizePolicy().hasHeightForWidth());
        groupBoxConditions->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBoxConditions);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(groupBoxConditions);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 680, 162));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        verticalLayout_2->addWidget(groupBoxConditions);

        groupBoxOutput = new QGroupBox(VqbSchemaConstruct);
        groupBoxOutput->setObjectName(QString::fromUtf8("groupBoxOutput"));
        sizePolicy.setHeightForWidth(groupBoxOutput->sizePolicy().hasHeightForWidth());
        groupBoxOutput->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(groupBoxOutput);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBoxSubject = new QGroupBox(groupBoxOutput);
        groupBoxSubject->setObjectName(QString::fromUtf8("groupBoxSubject"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(groupBoxSubject->sizePolicy().hasHeightForWidth());
        groupBoxSubject->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(9);
        groupBoxSubject->setFont(font);
        groupBoxSubject->setFlat(false);
        groupBoxSubject->setCheckable(false);
        verticalLayout_3 = new QVBoxLayout(groupBoxSubject);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lineEditFilterSubject = new KLineEdit(groupBoxSubject);
        lineEditFilterSubject->setObjectName(QString::fromUtf8("lineEditFilterSubject"));
        lineEditFilterSubject->setEnabled(false);

        verticalLayout_3->addWidget(lineEditFilterSubject);

        listSubject = new KListWidget(groupBoxSubject);
        listSubject->setObjectName(QString::fromUtf8("listSubject"));

        verticalLayout_3->addWidget(listSubject);

        cbShowAllSubject = new QCheckBox(groupBoxSubject);
        cbShowAllSubject->setObjectName(QString::fromUtf8("cbShowAllSubject"));
        cbShowAllSubject->setEnabled(false);

        verticalLayout_3->addWidget(cbShowAllSubject);


        horizontalLayout->addWidget(groupBoxSubject);

        groupBoxPredicate = new QGroupBox(groupBoxOutput);
        groupBoxPredicate->setObjectName(QString::fromUtf8("groupBoxPredicate"));
        sizePolicy1.setHeightForWidth(groupBoxPredicate->sizePolicy().hasHeightForWidth());
        groupBoxPredicate->setSizePolicy(sizePolicy1);
        groupBoxPredicate->setFont(font);
        verticalLayout_4 = new QVBoxLayout(groupBoxPredicate);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lineEditFilterPredicate = new KLineEdit(groupBoxPredicate);
        lineEditFilterPredicate->setObjectName(QString::fromUtf8("lineEditFilterPredicate"));
        lineEditFilterPredicate->setEnabled(false);

        verticalLayout_4->addWidget(lineEditFilterPredicate);

        listPredicate = new KListWidget(groupBoxPredicate);
        listPredicate->setObjectName(QString::fromUtf8("listPredicate"));

        verticalLayout_4->addWidget(listPredicate);

        cbShowAllPredicate = new QCheckBox(groupBoxPredicate);
        cbShowAllPredicate->setObjectName(QString::fromUtf8("cbShowAllPredicate"));
        cbShowAllPredicate->setEnabled(false);

        verticalLayout_4->addWidget(cbShowAllPredicate);


        horizontalLayout->addWidget(groupBoxPredicate);

        groupBoxObject = new QGroupBox(groupBoxOutput);
        groupBoxObject->setObjectName(QString::fromUtf8("groupBoxObject"));
        sizePolicy1.setHeightForWidth(groupBoxObject->sizePolicy().hasHeightForWidth());
        groupBoxObject->setSizePolicy(sizePolicy1);
        groupBoxObject->setFont(font);
        verticalLayout_5 = new QVBoxLayout(groupBoxObject);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        lineEditFilterObject = new KLineEdit(groupBoxObject);
        lineEditFilterObject->setObjectName(QString::fromUtf8("lineEditFilterObject"));
        lineEditFilterObject->setEnabled(false);

        verticalLayout_5->addWidget(lineEditFilterObject);

        listObject = new KListWidget(groupBoxObject);
        listObject->setObjectName(QString::fromUtf8("listObject"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(listObject->sizePolicy().hasHeightForWidth());
        listObject->setSizePolicy(sizePolicy2);

        verticalLayout_5->addWidget(listObject);

        cbShowAllObject = new QCheckBox(groupBoxObject);
        cbShowAllObject->setObjectName(QString::fromUtf8("cbShowAllObject"));
        cbShowAllObject->setEnabled(false);

        verticalLayout_5->addWidget(cbShowAllObject);

        listObject->raise();
        lineEditFilterObject->raise();
        cbShowAllObject->raise();

        horizontalLayout->addWidget(groupBoxObject);

        listBoxConditions = new KEditListBox(groupBoxOutput);
        listBoxConditions->setObjectName(QString::fromUtf8("listBoxConditions"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(3);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(listBoxConditions->sizePolicy().hasHeightForWidth());
        listBoxConditions->setSizePolicy(sizePolicy3);
        listBoxConditions->setFont(font);
        listBoxConditions->setFlat(false);

        horizontalLayout->addWidget(listBoxConditions);


        verticalLayout_2->addWidget(groupBoxOutput);

        QWidget::setTabOrder(scrollArea, lineEditFilterSubject);
        QWidget::setTabOrder(lineEditFilterSubject, listSubject);
        QWidget::setTabOrder(listSubject, cbShowAllSubject);
        QWidget::setTabOrder(cbShowAllSubject, lineEditFilterPredicate);
        QWidget::setTabOrder(lineEditFilterPredicate, listPredicate);
        QWidget::setTabOrder(listPredicate, cbShowAllPredicate);
        QWidget::setTabOrder(cbShowAllPredicate, lineEditFilterObject);
        QWidget::setTabOrder(lineEditFilterObject, listObject);
        QWidget::setTabOrder(listObject, cbShowAllObject);

        retranslateUi(VqbSchemaConstruct);

        QMetaObject::connectSlotsByName(VqbSchemaConstruct);
    } // setupUi

    void retranslateUi(QWidget *VqbSchemaConstruct)
    {
        VqbSchemaConstruct->setWindowTitle(QApplication::translate("VqbSchemaConstruct", "Form", 0, QApplication::UnicodeUTF8));
        groupBoxConditions->setTitle(QApplication::translate("VqbSchemaConstruct", "Conditions", 0, QApplication::UnicodeUTF8));
        groupBoxOutput->setTitle(QApplication::translate("VqbSchemaConstruct", "Output", 0, QApplication::UnicodeUTF8));
        groupBoxSubject->setTitle(QApplication::translate("VqbSchemaConstruct", "Subject", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        lineEditFilterSubject->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Filters Subjects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        lineEditFilterSubject->setClickMessage(QApplication::translate("VqbSchemaConstruct", "<Filter>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        listSubject->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Subject List", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        cbShowAllSubject->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Show All Subjects from System (with a limited number)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbShowAllSubject->setText(QApplication::translate("VqbSchemaConstruct", "Show All", 0, QApplication::UnicodeUTF8));
        groupBoxPredicate->setTitle(QApplication::translate("VqbSchemaConstruct", "Predicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        lineEditFilterPredicate->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Filters Predicates", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        lineEditFilterPredicate->setClickMessage(QApplication::translate("VqbSchemaConstruct", "<Filter>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        listPredicate->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Subject List", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        cbShowAllPredicate->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Show All Predicates from System (with a limited number)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbShowAllPredicate->setText(QApplication::translate("VqbSchemaConstruct", "Show All", 0, QApplication::UnicodeUTF8));
        groupBoxObject->setTitle(QApplication::translate("VqbSchemaConstruct", "Object", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        lineEditFilterObject->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Filter Objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        lineEditFilterObject->setClickMessage(QApplication::translate("VqbSchemaConstruct", "<Filter>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        listObject->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Object List", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        cbShowAllObject->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Show All Objects from System (with a limited number)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbShowAllObject->setText(QApplication::translate("VqbSchemaConstruct", "Show All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listBoxConditions->setToolTip(QApplication::translate("VqbSchemaConstruct", "Output Triples", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        listBoxConditions->setStatusTip(QApplication::translate("VqbSchemaConstruct", "Output Triples", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        listBoxConditions->setTitle(QApplication::translate("VqbSchemaConstruct", "Graph Pattern", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VqbSchemaConstruct: public Ui_VqbSchemaConstruct {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VQBSCHEMACONSTRUCT_H
