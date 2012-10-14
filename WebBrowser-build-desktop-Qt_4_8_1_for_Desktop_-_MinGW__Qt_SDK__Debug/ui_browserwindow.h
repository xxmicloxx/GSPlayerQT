/********************************************************************************
** Form generated from reading UI file 'browserwindow.ui'
**
** Created: Fri 5. Oct 16:07:12 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BROWSERWINDOW_H
#define UI_BROWSERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_BrowserWindow
{
public:
    QAction *actionBeenden;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QWebView *webView;
    QMenuBar *menuBar;
    QMenu *menuDatei;

    void setupUi(QMainWindow *BrowserWindow)
    {
        if (BrowserWindow->objectName().isEmpty())
            BrowserWindow->setObjectName(QString::fromUtf8("BrowserWindow"));
        BrowserWindow->resize(752, 480);
        actionBeenden = new QAction(BrowserWindow);
        actionBeenden->setObjectName(QString::fromUtf8("actionBeenden"));
        centralWidget = new QWidget(BrowserWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        webView = new QWebView(centralWidget);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl(QString::fromUtf8("http://www.google.de/")));

        verticalLayout->addWidget(webView);

        BrowserWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(BrowserWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 752, 21));
        menuDatei = new QMenu(menuBar);
        menuDatei->setObjectName(QString::fromUtf8("menuDatei"));
        BrowserWindow->setMenuBar(menuBar);

        menuBar->addAction(menuDatei->menuAction());
        menuDatei->addAction(actionBeenden);

        retranslateUi(BrowserWindow);
        QObject::connect(actionBeenden, SIGNAL(activated()), BrowserWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(BrowserWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BrowserWindow)
    {
        BrowserWindow->setWindowTitle(QApplication::translate("BrowserWindow", "BrowserWindow", 0, QApplication::UnicodeUTF8));
        actionBeenden->setText(QApplication::translate("BrowserWindow", "Beenden", 0, QApplication::UnicodeUTF8));
        menuDatei->setTitle(QApplication::translate("BrowserWindow", "Datei", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BrowserWindow: public Ui_BrowserWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWSERWINDOW_H
