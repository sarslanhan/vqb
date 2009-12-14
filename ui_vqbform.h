/********************************************************************************
** Form generated from reading ui file 'vqbform.ui'
**
** Created: Wed Oct 14 18:34:07 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VQBFORM_H
#define UI_VQBFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QScrollArea>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include "kpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_VqbFormClass
{
public:
    QLabel *label;
    QLabel *label_3;
    QTabWidget *tabWidget;
    QWidget *tabQuery;
    QTextEdit *queryViewer;
    QWidget *tabResults;
    QTextEdit *queryResults;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QListWidget *outputList;
    KPushButton *buttonUp;
    KPushButton *buttonDown;
    KPushButton *buttonRemove;

    void setupUi(QWidget *VqbFormClass)
    {
        if (VqbFormClass->objectName().isEmpty())
            VqbFormClass->setObjectName(QString::fromUtf8("VqbFormClass"));
        VqbFormClass->resize(782, 655);
        QFont font;
        font.setPointSize(9);
        VqbFormClass->setFont(font);
        label = new QLabel(VqbFormClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 491, 61));
        label_3 = new QLabel(VqbFormClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 421, 131, 16));
        tabWidget = new QTabWidget(VqbFormClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(200, 420, 561, 221));
        tabQuery = new QWidget();
        tabQuery->setObjectName(QString::fromUtf8("tabQuery"));
        queryViewer = new QTextEdit(tabQuery);
        queryViewer->setObjectName(QString::fromUtf8("queryViewer"));
        queryViewer->setGeometry(QRect(10, 10, 541, 180));
        queryViewer->setFont(font);
        tabWidget->addTab(tabQuery, QString());
        tabResults = new QWidget();
        tabResults->setObjectName(QString::fromUtf8("tabResults"));
        queryResults = new QTextEdit(tabResults);
        queryResults->setObjectName(QString::fromUtf8("queryResults"));
        queryResults->setGeometry(QRect(10, 10, 541, 180));
        tabWidget->addTab(tabResults, QString());
        scrollArea = new QScrollArea(VqbFormClass);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(20, 80, 740, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 736, 317));
        scrollArea->setWidget(scrollAreaWidgetContents);
        outputList = new QListWidget(VqbFormClass);
        outputList->setObjectName(QString::fromUtf8("outputList"));
        outputList->setGeometry(QRect(20, 450, 121, 191));
        outputList->setFont(font);
        buttonUp = new KPushButton(VqbFormClass);
        buttonUp->setObjectName(QString::fromUtf8("buttonUp"));
        buttonUp->setGeometry(QRect(150, 491, 21, 20));
        buttonDown = new KPushButton(VqbFormClass);
        buttonDown->setObjectName(QString::fromUtf8("buttonDown"));
        buttonDown->setGeometry(QRect(150, 520, 21, 20));
        buttonRemove = new KPushButton(VqbFormClass);
        buttonRemove->setObjectName(QString::fromUtf8("buttonRemove"));
        buttonRemove->setGeometry(QRect(150, 550, 21, 20));

        retranslateUi(VqbFormClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(VqbFormClass);
    } // setupUi

    void retranslateUi(QWidget *VqbFormClass)
    {
        VqbFormClass->setWindowTitle(QApplication::translate("VqbFormClass", "VqbForm", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VqbFormClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:600;\">Query:</span><span style=\" font-family:'DejaVu Sans'; font-size:9pt;\"> Please specify Type of data to query. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'DejaVu Sans'; font-size:9pt;\">	- use + to add restrictions</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" fon"
                        "t-family:'DejaVu Sans'; font-size:9pt;\">	- add new Query Trees if needed</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VqbFormClass", "Output variables:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabQuery), QApplication::translate("VqbFormClass", "Resulting Query", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabResults), QApplication::translate("VqbFormClass", "Preview Query Results", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonUp->setToolTip(QApplication::translate("VqbFormClass", "Move Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonDown->setToolTip(QApplication::translate("VqbFormClass", "Move Down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonRemove->setToolTip(QApplication::translate("VqbFormClass", "Remove", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Q_UNUSED(VqbFormClass);
    } // retranslateUi

};

namespace Ui {
    class VqbFormClass: public Ui_VqbFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VQBFORM_H
