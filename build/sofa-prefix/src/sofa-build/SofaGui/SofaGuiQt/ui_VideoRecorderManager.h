/********************************************************************************
** Form generated from reading UI file 'VideoRecorderManager.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEORECORDERMANAGER_H
#define UI_VIDEORECORDERMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VideoRecorderManager
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *RecordingTypeButtonGroup;
    QRadioButton *ScreenshotsRecordingTypeRadioButton;
    QRadioButton *MovieRecordingTypeRadioButton;
    QGroupBox *VideoRecorderOptionGroupBox;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer2;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonClose;

    void setupUi(QDialog *VideoRecorderManager)
    {
        if (VideoRecorderManager->objectName().isEmpty())
            VideoRecorderManager->setObjectName(QStringLiteral("VideoRecorderManager"));
        VideoRecorderManager->resize(600, 400);
        vboxLayout = new QVBoxLayout(VideoRecorderManager);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        RecordingTypeButtonGroup = new QGroupBox(VideoRecorderManager);
        RecordingTypeButtonGroup->setObjectName(QStringLiteral("RecordingTypeButtonGroup"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RecordingTypeButtonGroup->sizePolicy().hasHeightForWidth());
        RecordingTypeButtonGroup->setSizePolicy(sizePolicy);
        RecordingTypeButtonGroup->setMinimumSize(QSize(0, 60));
        ScreenshotsRecordingTypeRadioButton = new QRadioButton(RecordingTypeButtonGroup);
        ScreenshotsRecordingTypeRadioButton->setObjectName(QStringLiteral("ScreenshotsRecordingTypeRadioButton"));
        ScreenshotsRecordingTypeRadioButton->setGeometry(QRect(20, 20, 380, 20));
        ScreenshotsRecordingTypeRadioButton->setChecked(true);
        MovieRecordingTypeRadioButton = new QRadioButton(RecordingTypeButtonGroup);
        MovieRecordingTypeRadioButton->setObjectName(QStringLiteral("MovieRecordingTypeRadioButton"));
        MovieRecordingTypeRadioButton->setGeometry(QRect(20, 40, 430, 16));

        vboxLayout->addWidget(RecordingTypeButtonGroup);

        VideoRecorderOptionGroupBox = new QGroupBox(VideoRecorderManager);
        VideoRecorderOptionGroupBox->setObjectName(QStringLiteral("VideoRecorderOptionGroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(VideoRecorderOptionGroupBox->sizePolicy().hasHeightForWidth());
        VideoRecorderOptionGroupBox->setSizePolicy(sizePolicy1);
        hboxLayout = new QHBoxLayout(VideoRecorderOptionGroupBox);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));

        vboxLayout->addWidget(VideoRecorderOptionGroupBox);

        spacer2 = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer2);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QStringLiteral("hboxLayout1"));
        Horizontal_Spacing2 = new QSpacerItem(237, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(Horizontal_Spacing2);

        buttonClose = new QPushButton(VideoRecorderManager);
        buttonClose->setObjectName(QStringLiteral("buttonClose"));
        buttonClose->setAutoDefault(true);

        hboxLayout1->addWidget(buttonClose);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(VideoRecorderManager);
        QObject::connect(buttonClose, SIGNAL(clicked()), VideoRecorderManager, SLOT(close()));
        QObject::connect(RecordingTypeButtonGroup, SIGNAL(clicked(bool)), VideoRecorderManager, SLOT(onChangeRecordingType()));

        QMetaObject::connectSlotsByName(VideoRecorderManager);
    } // setupUi

    void retranslateUi(QDialog *VideoRecorderManager)
    {
        VideoRecorderManager->setWindowTitle(QApplication::translate("VideoRecorderManager", "VideoRecorderManager", 0));
        RecordingTypeButtonGroup->setTitle(QApplication::translate("VideoRecorderManager", "RecordingType", 0));
        ScreenshotsRecordingTypeRadioButton->setText(QApplication::translate("VideoRecorderManager", "List of Screenshots", 0));
        MovieRecordingTypeRadioButton->setText(QApplication::translate("VideoRecorderManager", "Movie file encoded with FFMPEG", 0));
        VideoRecorderOptionGroupBox->setTitle(QApplication::translate("VideoRecorderManager", "Options", 0));
        buttonClose->setText(QApplication::translate("VideoRecorderManager", "&Close", 0));
        buttonClose->setShortcut(QApplication::translate("VideoRecorderManager", "Alt+C", 0));
    } // retranslateUi

};

namespace Ui {
    class VideoRecorderManager: public Ui_VideoRecorderManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEORECORDERMANAGER_H
