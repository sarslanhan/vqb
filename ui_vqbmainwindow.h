/********************************************************************************
** Form generated from reading UI file 'vqbmainwindow.ui'
**
** Created: Fri Mar 12 17:00:47 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VQBMAINWINDOW_H
#define UI_VQBMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VqbMainWindow
{
public:
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionPostToPastebin;
    QAction *action_Return_to_Startup;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuWeb;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tabQuery;
    QVBoxLayout *verticalLayout_3;
    QTextEdit *queryViewer;
    QWidget *tabResults;
    QVBoxLayout *verticalLayout_4;
    QTextEdit *queryResults;

    void setupUi(QMainWindow *VqbMainWindow)
    {
        if (VqbMainWindow->objectName().isEmpty())
            VqbMainWindow->setObjectName(QString::fromUtf8("VqbMainWindow"));
        VqbMainWindow->resize(779, 813);
        QFont font;
        font.setPointSize(9);
        VqbMainWindow->setFont(font);
        actionSave = new QAction(VqbMainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(VqbMainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionPostToPastebin = new QAction(VqbMainWindow);
        actionPostToPastebin->setObjectName(QString::fromUtf8("actionPostToPastebin"));
        action_Return_to_Startup = new QAction(VqbMainWindow);
        action_Return_to_Startup->setObjectName(QString::fromUtf8("action_Return_to_Startup"));
        centralWidget = new QWidget(VqbMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShadow(QFrame::Raised);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 757, 464));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        VqbMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VqbMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 779, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuWeb = new QMenu(menuBar);
        menuWeb->setObjectName(QString::fromUtf8("menuWeb"));
        VqbMainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(VqbMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        VqbMainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(VqbMainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setBaseSize(QSize(779, 248));
        dockWidget->setFloating(false);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        dockWidgetContents_4->setBaseSize(QSize(779, 226));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(dockWidgetContents_4);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setBaseSize(QSize(751, 221));
        tabQuery = new QWidget();
        tabQuery->setObjectName(QString::fromUtf8("tabQuery"));
        verticalLayout_3 = new QVBoxLayout(tabQuery);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        queryViewer = new QTextEdit(tabQuery);
        queryViewer->setObjectName(QString::fromUtf8("queryViewer"));
        queryViewer->setFont(font);
        queryViewer->setLineWrapMode(QTextEdit::NoWrap);

        verticalLayout_3->addWidget(queryViewer);

        tabWidget->addTab(tabQuery, QString());
        tabResults = new QWidget();
        tabResults->setObjectName(QString::fromUtf8("tabResults"));
        verticalLayout_4 = new QVBoxLayout(tabResults);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        queryResults = new QTextEdit(tabResults);
        queryResults->setObjectName(QString::fromUtf8("queryResults"));
        queryResults->setLineWrapMode(QTextEdit::NoWrap);

        verticalLayout_4->addWidget(queryResults);

        tabWidget->addTab(tabResults, QString());

        verticalLayout_2->addWidget(tabWidget);

        dockWidget->setWidget(dockWidgetContents_4);
        VqbMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget);
        QWidget::setTabOrder(scrollArea, tabWidget);
        QWidget::setTabOrder(tabWidget, queryViewer);
        QWidget::setTabOrder(queryViewer, queryResults);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuWeb->menuAction());
        menu_File->addAction(actionSave);
        menu_File->addAction(actionExit);
        menu_File->addSeparator();
        menu_File->addAction(action_Return_to_Startup);
        menuWeb->addAction(actionPostToPastebin);

        retranslateUi(VqbMainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(VqbMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *VqbMainWindow)
    {
        VqbMainWindow->setWindowTitle(QApplication::translate("VqbMainWindow", "VqbForm", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("VqbMainWindow", "&Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionSave->setStatusTip(QApplication::translate("VqbMainWindow", "Saves the query to a file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave->setShortcut(QApplication::translate("VqbMainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("VqbMainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionExit->setStatusTip(QApplication::translate("VqbMainWindow", "Exits the Application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionPostToPastebin->setText(QApplication::translate("VqbMainWindow", "Post to pastebin.com", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionPostToPastebin->setStatusTip(QApplication::translate("VqbMainWindow", "Post to pastebin.com", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionPostToPastebin->setShortcut(QApplication::translate("VqbMainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        action_Return_to_Startup->setText(QApplication::translate("VqbMainWindow", "&Return to Startup", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_Return_to_Startup->setStatusTip(QApplication::translate("VqbMainWindow", "Returns to the query bulding mode selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Return_to_Startup->setShortcut(QApplication::translate("VqbMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("VqbMainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuWeb->setTitle(QApplication::translate("VqbMainWindow", "&Web", 0, QApplication::UnicodeUTF8));
        dockWidget->setWindowTitle(QApplication::translate("VqbMainWindow", "Preview", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tabWidget->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        tabQuery->setStatusTip(QApplication::translate("VqbMainWindow", "Displays current query", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tabWidget->setTabText(tabWidget->indexOf(tabQuery), QApplication::translate("VqbMainWindow", "Resulting Query", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tabResults->setStatusTip(QApplication::translate("VqbMainWindow", "Test-runs the query", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tabWidget->setTabText(tabWidget->indexOf(tabResults), QApplication::translate("VqbMainWindow", "Query Results", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VqbMainWindow: public Ui_VqbMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VQBMAINWINDOW_H
