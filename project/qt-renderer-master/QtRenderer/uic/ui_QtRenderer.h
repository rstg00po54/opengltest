/********************************************************************************
** Form generated from reading UI file 'QtRenderer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRENDERER_H
#define UI_QTRENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Renderer.h"

QT_BEGIN_NAMESPACE

class Ui_QtRendererClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *addButton;
    QPushButton *rmButton;
    QVBoxLayout *verticalLayout_2;
    Renderer *rendererLabel;
    QGroupBox *groupBox_2;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *QtRendererClass)
    {
        if (QtRendererClass->objectName().isEmpty())
            QtRendererClass->setObjectName(QString::fromUtf8("QtRendererClass"));
        QtRendererClass->resize(1280, 800);
        QtRendererClass->setMinimumSize(QSize(1280, 800));
        centralWidget = new QWidget(QtRendererClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(200, 0));
        groupBox->setMaximumSize(QSize(400, 16777215));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        addButton = new QPushButton(groupBox);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        horizontalLayout_2->addWidget(addButton);

        rmButton = new QPushButton(groupBox);
        rmButton->setObjectName(QString::fromUtf8("rmButton"));

        horizontalLayout_2->addWidget(rmButton);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout->addWidget(groupBox);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        rendererLabel = new Renderer(centralWidget);
        rendererLabel->setObjectName(QString::fromUtf8("rendererLabel"));

        verticalLayout_2->addWidget(rendererLabel);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));

        verticalLayout_2->addWidget(groupBox_2);


        horizontalLayout->addLayout(verticalLayout_2);

        QtRendererClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtRendererClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtRendererClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(QtRendererClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        QtRendererClass->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());

        retranslateUi(QtRendererClass);

        QMetaObject::connectSlotsByName(QtRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtRendererClass)
    {
        QtRendererClass->setWindowTitle(QApplication::translate("QtRendererClass", "QtRenderer", nullptr));
        groupBox->setTitle(QApplication::translate("QtRendererClass", "Scene", nullptr));
        addButton->setText(QApplication::translate("QtRendererClass", "Add", nullptr));
        rmButton->setText(QApplication::translate("QtRendererClass", "Remove", nullptr));
        rendererLabel->setText(QString());
        groupBox_2->setTitle(QApplication::translate("QtRendererClass", "Setting", nullptr));
        menuFile->setTitle(QApplication::translate("QtRendererClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtRendererClass: public Ui_QtRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRENDERER_H
