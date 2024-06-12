/********************************************************************************
** Form generated from reading UI file 'AddDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDDIALOG_H
#define UI_ADDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_AddDialog
{
public:
    QLabel *label;
    QToolButton *toolButton;
    QLabel *label_2;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QLabel *label_3;
    QLabel *modelFile;
    QLabel *textureFile;
    QLabel *normalFile;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *AddDialog)
    {
        if (AddDialog->objectName().isEmpty())
            AddDialog->setObjectName(QString::fromUtf8("AddDialog"));
        AddDialog->resize(400, 250);
        AddDialog->setModal(true);
        label = new QLabel(AddDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 40, 91, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        label->setFont(font);
        toolButton = new QToolButton(AddDialog);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(300, 40, 41, 20));
        label_2 = new QLabel(AddDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 90, 91, 21));
        label_2->setFont(font);
        toolButton_2 = new QToolButton(AddDialog);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setGeometry(QRect(300, 90, 41, 20));
        toolButton_3 = new QToolButton(AddDialog);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));
        toolButton_3->setGeometry(QRect(300, 140, 41, 20));
        label_3 = new QLabel(AddDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 140, 91, 21));
        label_3->setFont(font);
        modelFile = new QLabel(AddDialog);
        modelFile->setObjectName(QString::fromUtf8("modelFile"));
        modelFile->setGeometry(QRect(140, 40, 151, 21));
        modelFile->setFont(font);
        textureFile = new QLabel(AddDialog);
        textureFile->setObjectName(QString::fromUtf8("textureFile"));
        textureFile->setGeometry(QRect(140, 90, 151, 21));
        textureFile->setFont(font);
        normalFile = new QLabel(AddDialog);
        normalFile->setObjectName(QString::fromUtf8("normalFile"));
        normalFile->setGeometry(QRect(140, 140, 151, 21));
        normalFile->setFont(font);
        okBtn = new QPushButton(AddDialog);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(70, 200, 99, 28));
        cancelBtn = new QPushButton(AddDialog);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setGeometry(QRect(220, 200, 99, 28));

        retranslateUi(AddDialog);

        QMetaObject::connectSlotsByName(AddDialog);
    } // setupUi

    void retranslateUi(QDialog *AddDialog)
    {
        AddDialog->setWindowTitle(QApplication::translate("AddDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("AddDialog", "\346\250\241\345\236\213\346\226\207\344\273\266\357\274\232", nullptr));
        toolButton->setText(QApplication::translate("AddDialog", "...", nullptr));
        label_2->setText(QApplication::translate("AddDialog", "\347\272\271\347\220\206\350\264\264\345\233\276\357\274\232", nullptr));
        toolButton_2->setText(QApplication::translate("AddDialog", "...", nullptr));
        toolButton_3->setText(QApplication::translate("AddDialog", "...", nullptr));
        label_3->setText(QApplication::translate("AddDialog", "\346\263\225\347\272\277\350\264\264\345\233\276\357\274\232", nullptr));
        modelFile->setText(QApplication::translate("AddDialog", "-", nullptr));
        textureFile->setText(QApplication::translate("AddDialog", "-", nullptr));
        normalFile->setText(QApplication::translate("AddDialog", "-", nullptr));
        okBtn->setText(QApplication::translate("AddDialog", "OK", nullptr));
        cancelBtn->setText(QApplication::translate("AddDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddDialog: public Ui_AddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDIALOG_H
