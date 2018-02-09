/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "nemofacerecogwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    NemoFaceRecogWidget *faceRecogWidget;
    QHBoxLayout *horizontalLayout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(955, 675);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        faceRecogWidget = new NemoFaceRecogWidget(centralWidget);
        if (faceRecogWidget->columnCount() < 5)
            faceRecogWidget->setColumnCount(5);
        if (faceRecogWidget->rowCount() < 4)
            faceRecogWidget->setRowCount(4);
        QFont font;
        font.setPointSize(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        faceRecogWidget->setItem(0, 0, __qtablewidgetitem);
        faceRecogWidget->setObjectName(QStringLiteral("faceRecogWidget"));
        faceRecogWidget->setEnabled(true);
        faceRecogWidget->setRowCount(4);
        faceRecogWidget->setColumnCount(5);
        faceRecogWidget->horizontalHeader()->setVisible(false);
        faceRecogWidget->horizontalHeader()->setCascadingSectionResizes(false);
        faceRecogWidget->horizontalHeader()->setDefaultSectionSize(100);
        faceRecogWidget->horizontalHeader()->setStretchLastSection(false);
        faceRecogWidget->verticalHeader()->setVisible(false);
        faceRecogWidget->verticalHeader()->setDefaultSectionSize(100);
        faceRecogWidget->verticalHeader()->setMinimumSectionSize(50);

        verticalLayout->addWidget(faceRecogWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));

        const bool __sortingEnabled = faceRecogWidget->isSortingEnabled();
        faceRecogWidget->setSortingEnabled(false);
        faceRecogWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
