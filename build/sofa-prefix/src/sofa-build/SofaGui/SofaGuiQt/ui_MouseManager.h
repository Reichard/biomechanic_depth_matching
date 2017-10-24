/********************************************************************************
** Form generated from reading UI file 'MouseManager.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOUSEMANAGER_H
#define UI_MOUSEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MouseManager
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *LeftButton;
    QVBoxLayout *vboxLayout1;
    QLabel *LeftOperationLabel;
    QComboBox *LeftOperationCombo;
    QGroupBox *MiddleButton;
    QVBoxLayout *vboxLayout2;
    QLabel *MiddleOperationLabel;
    QComboBox *MiddleOperationCombo;
    QGroupBox *RightButton;
    QVBoxLayout *vboxLayout3;
    QLabel *RightOperationLabel;
    QComboBox *RightOperationCombo;
    QSpacerItem *spacer2;

    void setupUi(QDialog *MouseManager)
    {
        if (MouseManager->objectName().isEmpty())
            MouseManager->setObjectName(QStringLiteral("MouseManager"));
        MouseManager->resize(295, 612);
        vboxLayout = new QVBoxLayout(MouseManager);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        LeftButton = new QGroupBox(MouseManager);
        LeftButton->setObjectName(QStringLiteral("LeftButton"));
        vboxLayout1 = new QVBoxLayout(LeftButton);
        vboxLayout1->setSpacing(6);
        vboxLayout1->setContentsMargins(11, 11, 11, 11);
        vboxLayout1->setObjectName(QStringLiteral("vboxLayout1"));
        LeftOperationLabel = new QLabel(LeftButton);
        LeftOperationLabel->setObjectName(QStringLiteral("LeftOperationLabel"));
        LeftOperationLabel->setWordWrap(false);

        vboxLayout1->addWidget(LeftOperationLabel);

        LeftOperationCombo = new QComboBox(LeftButton);
        LeftOperationCombo->setObjectName(QStringLiteral("LeftOperationCombo"));

        vboxLayout1->addWidget(LeftOperationCombo);


        vboxLayout->addWidget(LeftButton);

        MiddleButton = new QGroupBox(MouseManager);
        MiddleButton->setObjectName(QStringLiteral("MiddleButton"));
        vboxLayout2 = new QVBoxLayout(MiddleButton);
        vboxLayout2->setSpacing(6);
        vboxLayout2->setContentsMargins(11, 11, 11, 11);
        vboxLayout2->setObjectName(QStringLiteral("vboxLayout2"));
        MiddleOperationLabel = new QLabel(MiddleButton);
        MiddleOperationLabel->setObjectName(QStringLiteral("MiddleOperationLabel"));
        MiddleOperationLabel->setWordWrap(false);

        vboxLayout2->addWidget(MiddleOperationLabel);

        MiddleOperationCombo = new QComboBox(MiddleButton);
        MiddleOperationCombo->setObjectName(QStringLiteral("MiddleOperationCombo"));

        vboxLayout2->addWidget(MiddleOperationCombo);


        vboxLayout->addWidget(MiddleButton);

        RightButton = new QGroupBox(MouseManager);
        RightButton->setObjectName(QStringLiteral("RightButton"));
        vboxLayout3 = new QVBoxLayout(RightButton);
        vboxLayout3->setSpacing(6);
        vboxLayout3->setContentsMargins(11, 11, 11, 11);
        vboxLayout3->setObjectName(QStringLiteral("vboxLayout3"));
        RightOperationLabel = new QLabel(RightButton);
        RightOperationLabel->setObjectName(QStringLiteral("RightOperationLabel"));
        RightOperationLabel->setWordWrap(false);

        vboxLayout3->addWidget(RightOperationLabel);

        RightOperationCombo = new QComboBox(RightButton);
        RightOperationCombo->setObjectName(QStringLiteral("RightOperationCombo"));

        vboxLayout3->addWidget(RightOperationCombo);


        vboxLayout->addWidget(RightButton);

        spacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer2);


        retranslateUi(MouseManager);

        QMetaObject::connectSlotsByName(MouseManager);
    } // setupUi

    void retranslateUi(QDialog *MouseManager)
    {
        MouseManager->setWindowTitle(QApplication::translate("MouseManager", "MouseManager", 0));
        LeftButton->setTitle(QApplication::translate("MouseManager", "Left Button", 0));
        LeftOperationLabel->setText(QApplication::translate("MouseManager", "Operation:", 0));
        MiddleButton->setTitle(QApplication::translate("MouseManager", "Middle Button", 0));
        MiddleOperationLabel->setText(QApplication::translate("MouseManager", "Operation:", 0));
        RightButton->setTitle(QApplication::translate("MouseManager", "Right Button", 0));
        RightOperationLabel->setText(QApplication::translate("MouseManager", "Operation:", 0));
    } // retranslateUi

};

namespace Ui {
    class MouseManager: public Ui_MouseManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOUSEMANAGER_H
