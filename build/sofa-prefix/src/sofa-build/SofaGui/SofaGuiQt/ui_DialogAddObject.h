/********************************************************************************
** Form generated from reading UI file 'DialogAddObject.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGADDOBJECT_H
#define UI_DIALOGADDOBJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogAddObject
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *scaleText;
    QLineEdit *scaleValue;
    QHBoxLayout *hboxLayout1;
    QLabel *openFileText;
    QLineEdit *openFilePath;
    QPushButton *openFileButton;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QHBoxLayout *hboxLayout3;
    QLabel *positionText;
    QLineEdit *positionX;
    QLineEdit *positionY;
    QLineEdit *positionZ;
    QHBoxLayout *hboxLayout4;
    QLabel *rotationText;
    QLineEdit *rotationX;
    QLineEdit *rotationY;
    QLineEdit *rotationZ;
    QGroupBox *buttonGroup;
    QGridLayout *gridLayout1;
    QRadioButton *custom;

    void setupUi(QDialog *DialogAddObject)
    {
        if (DialogAddObject->objectName().isEmpty())
            DialogAddObject->setObjectName(QStringLiteral("DialogAddObject"));
        DialogAddObject->resize(556, 264);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogAddObject->sizePolicy().hasHeightForWidth());
        DialogAddObject->setSizePolicy(sizePolicy);
        DialogAddObject->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(DialogAddObject);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        scaleText = new QLabel(DialogAddObject);
        scaleText->setObjectName(QStringLiteral("scaleText"));
        scaleText->setWordWrap(false);

        hboxLayout->addWidget(scaleText);

        scaleValue = new QLineEdit(DialogAddObject);
        scaleValue->setObjectName(QStringLiteral("scaleValue"));

        hboxLayout->addWidget(scaleValue);


        gridLayout->addLayout(hboxLayout, 4, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QStringLiteral("hboxLayout1"));
        openFileText = new QLabel(DialogAddObject);
        openFileText->setObjectName(QStringLiteral("openFileText"));
        openFileText->setWordWrap(false);

        hboxLayout1->addWidget(openFileText);

        openFilePath = new QLineEdit(DialogAddObject);
        openFilePath->setObjectName(QStringLiteral("openFilePath"));

        hboxLayout1->addWidget(openFilePath);

        openFileButton = new QPushButton(DialogAddObject);
        openFileButton->setObjectName(QStringLiteral("openFileButton"));

        hboxLayout1->addWidget(openFileButton);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QStringLiteral("hboxLayout2"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(DialogAddObject);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout2->addWidget(buttonOk);

        buttonCancel = new QPushButton(DialogAddObject);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout2->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout2, 5, 0, 1, 1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QStringLiteral("hboxLayout3"));
        positionText = new QLabel(DialogAddObject);
        positionText->setObjectName(QStringLiteral("positionText"));
        positionText->setWordWrap(false);

        hboxLayout3->addWidget(positionText);

        positionX = new QLineEdit(DialogAddObject);
        positionX->setObjectName(QStringLiteral("positionX"));

        hboxLayout3->addWidget(positionX);

        positionY = new QLineEdit(DialogAddObject);
        positionY->setObjectName(QStringLiteral("positionY"));

        hboxLayout3->addWidget(positionY);

        positionZ = new QLineEdit(DialogAddObject);
        positionZ->setObjectName(QStringLiteral("positionZ"));

        hboxLayout3->addWidget(positionZ);


        gridLayout->addLayout(hboxLayout3, 2, 0, 1, 1);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QStringLiteral("hboxLayout4"));
        rotationText = new QLabel(DialogAddObject);
        rotationText->setObjectName(QStringLiteral("rotationText"));
        rotationText->setWordWrap(false);

        hboxLayout4->addWidget(rotationText);

        rotationX = new QLineEdit(DialogAddObject);
        rotationX->setObjectName(QStringLiteral("rotationX"));

        hboxLayout4->addWidget(rotationX);

        rotationY = new QLineEdit(DialogAddObject);
        rotationY->setObjectName(QStringLiteral("rotationY"));

        hboxLayout4->addWidget(rotationY);

        rotationZ = new QLineEdit(DialogAddObject);
        rotationZ->setObjectName(QStringLiteral("rotationZ"));

        hboxLayout4->addWidget(rotationZ);


        gridLayout->addLayout(hboxLayout4, 3, 0, 1, 1);

        buttonGroup = new QGroupBox(DialogAddObject);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        gridLayout1 = new QGridLayout(buttonGroup);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QStringLiteral("gridLayout1"));
        custom = new QRadioButton(buttonGroup);
        custom->setObjectName(QStringLiteral("custom"));

        gridLayout1->addWidget(custom, 0, 0, 1, 1);


        gridLayout->addWidget(buttonGroup, 0, 0, 1, 1);


        retranslateUi(DialogAddObject);
        QObject::connect(openFileButton, SIGNAL(clicked()), DialogAddObject, SLOT(fileOpen()));
        QObject::connect(buttonOk, SIGNAL(clicked()), DialogAddObject, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), DialogAddObject, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogAddObject);
    } // setupUi

    void retranslateUi(QDialog *DialogAddObject)
    {
        DialogAddObject->setWindowTitle(QApplication::translate("DialogAddObject", "Add a scene or an object", 0));
        scaleText->setText(QApplication::translate("DialogAddObject", "Scale", 0));
        openFileText->setText(QApplication::translate("DialogAddObject", "Open File", 0));
        openFileButton->setText(QString());
        buttonOk->setText(QApplication::translate("DialogAddObject", "&OK", 0));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("DialogAddObject", "&Cancel", 0));
        buttonCancel->setShortcut(QString());
        positionText->setText(QApplication::translate("DialogAddObject", "Initial Position", 0));
        rotationText->setText(QApplication::translate("DialogAddObject", "Initial Rotation", 0));
        buttonGroup->setTitle(QApplication::translate("DialogAddObject", "Objects", 0));
        custom->setText(QApplication::translate("DialogAddObject", "custom", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogAddObject: public Ui_DialogAddObject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGADDOBJECT_H
