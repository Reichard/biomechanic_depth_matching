/********************************************************************************
** Form generated from reading UI file 'GUI.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "sofa/gui/qt/SofaGUIQt.h"

QT_BEGIN_NAMESPACE

class SOFA_SOFAGUIQT_API Ui_GUI
{
public:
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileReloadAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExitAction;
    QAction *helpContentsAction;
    QAction *helpIndexAction;
    QAction *helpAboutAction;
    QAction *ViewerAction;
    QAction *editRecordDirectoryAction;
    QAction *editGnuplotDirectoryAction;
    QAction *PluginManagerAction;
    QAction *MouseManagerAction;
    QAction *Action;
    QAction *TestAction;
    QAction *VideoRecorderManagerAction;
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *mainWidgetLayout;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *View;
    QMenu *helpMenu;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QWidget *optionTabs;
    QVBoxLayout *_13;
    QGridLayout *_14;
    QPushButton *ResetViewButton;
    QPushButton *startButton;
    QPushButton *SaveViewButton;
    QPushButton *ResetSceneButton;
    QPushButton *stepButton;
    QPushButton *screenshotButton;
    QHBoxLayout *_15;
    QLabel *dtLabel;
    QLineEdit *dtEdit;
    QCheckBox *realTimeCheckBox;
    QSpacerItem *spacer_tab;
    QTabWidget *tabs;
    QWidget *tabView;
    QGridLayout *gridLayout1;
    QGridLayout *_17;
    QGridLayout *_18;
    QSpacerItem *spacer8;
    QLabel *pixmapLabel1;
    QSpacerItem *spacer7;
    QWidget *TabGraph;
    QVBoxLayout *_19;
    QPushButton *ExportGraphButton;
    QWidget *TabVisualGraph;
    QVBoxLayout *_20;
    QPushButton *ExportVisualGraphButton;
    QWidget *TabStats;
    QVBoxLayout *_21;
    QCheckBox *dumpStateCheckBox;
    QCheckBox *displayComputationTimeCheckBox;
    QCheckBox *exportGnuplotFilesCheckbox;
    QCheckBox *exportVisitorCheckbox;
    QWidget *TabPage;
    QVBoxLayout *_22;
    QHBoxLayout *_23;
    QSpacerItem *spacer5_3_3;
    QSpinBox *sizeW;
    QLabel *textLabel_sizeX;
    QSpinBox *sizeH;
    QSpacerItem *spacer5_3_2_3;
    QTextBrowser *textEdit1;

    void setupUi(QMainWindow *GUI)
    {
        if (GUI->objectName().isEmpty())
            GUI->setObjectName(QStringLiteral("GUI"));
        GUI->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GUI->sizePolicy().hasHeightForWidth());
        GUI->setSizePolicy(sizePolicy);
        GUI->setMinimumSize(QSize(210, 481));
        GUI->setAcceptDrops(true);
        fileNewAction = new QAction(GUI);
        fileNewAction->setObjectName(QStringLiteral("fileNewAction"));
        QIcon icon;
        icon.addFile(QStringLiteral("image2"), QSize(), QIcon::Normal, QIcon::Off);
        fileNewAction->setIcon(icon);
        fileNewAction->setProperty("name", QVariant(QByteArray("fileNewAction")));
        fileOpenAction = new QAction(GUI);
        fileOpenAction->setObjectName(QStringLiteral("fileOpenAction"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("image3"), QSize(), QIcon::Normal, QIcon::Off);
        fileOpenAction->setIcon(icon1);
        fileOpenAction->setProperty("name", QVariant(QByteArray("fileOpenAction")));
        fileReloadAction = new QAction(GUI);
        fileReloadAction->setObjectName(QStringLiteral("fileReloadAction"));
        fileReloadAction->setProperty("name", QVariant(QByteArray("fileReloadAction")));
        fileSaveAction = new QAction(GUI);
        fileSaveAction->setObjectName(QStringLiteral("fileSaveAction"));
        fileSaveAction->setEnabled(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("image4"), QSize(), QIcon::Normal, QIcon::Off);
        fileSaveAction->setIcon(icon2);
        fileSaveAction->setProperty("name", QVariant(QByteArray("fileSaveAction")));
        fileSaveAsAction = new QAction(GUI);
        fileSaveAsAction->setObjectName(QStringLiteral("fileSaveAsAction"));
        fileSaveAsAction->setProperty("name", QVariant(QByteArray("fileSaveAsAction")));
        fileExitAction = new QAction(GUI);
        fileExitAction->setObjectName(QStringLiteral("fileExitAction"));
        fileExitAction->setProperty("name", QVariant(QByteArray("fileExitAction")));
        helpContentsAction = new QAction(GUI);
        helpContentsAction->setObjectName(QStringLiteral("helpContentsAction"));
        helpContentsAction->setEnabled(false);
        helpContentsAction->setProperty("name", QVariant(QByteArray("helpContentsAction")));
        helpIndexAction = new QAction(GUI);
        helpIndexAction->setObjectName(QStringLiteral("helpIndexAction"));
        helpIndexAction->setEnabled(false);
        helpIndexAction->setProperty("name", QVariant(QByteArray("helpIndexAction")));
        helpAboutAction = new QAction(GUI);
        helpAboutAction->setObjectName(QStringLiteral("helpAboutAction"));
        helpAboutAction->setEnabled(false);
        helpAboutAction->setProperty("name", QVariant(QByteArray("helpAboutAction")));
        ViewerAction = new QAction(GUI);
        ViewerAction->setObjectName(QStringLiteral("ViewerAction"));
        ViewerAction->setProperty("name", QVariant(QByteArray("ViewerAction")));
        editRecordDirectoryAction = new QAction(GUI);
        editRecordDirectoryAction->setObjectName(QStringLiteral("editRecordDirectoryAction"));
        editRecordDirectoryAction->setProperty("name", QVariant(QByteArray("editRecordDirectoryAction")));
        editGnuplotDirectoryAction = new QAction(GUI);
        editGnuplotDirectoryAction->setObjectName(QStringLiteral("editGnuplotDirectoryAction"));
        editGnuplotDirectoryAction->setProperty("name", QVariant(QByteArray("editGnuplotDirectoryAction")));
        PluginManagerAction = new QAction(GUI);
        PluginManagerAction->setObjectName(QStringLiteral("PluginManagerAction"));
        PluginManagerAction->setProperty("name", QVariant(QByteArray("PluginManagerAction")));
        MouseManagerAction = new QAction(GUI);
        MouseManagerAction->setObjectName(QStringLiteral("MouseManagerAction"));
        MouseManagerAction->setProperty("name", QVariant(QByteArray("MouseManagerAction")));
        Action = new QAction(GUI);
        Action->setObjectName(QStringLiteral("Action"));
        Action->setProperty("name", QVariant(QByteArray("Action")));
        TestAction = new QAction(GUI);
        TestAction->setObjectName(QStringLiteral("TestAction"));
        TestAction->setProperty("name", QVariant(QByteArray("TestAction")));
        VideoRecorderManagerAction = new QAction(GUI);
        VideoRecorderManagerAction->setObjectName(QStringLiteral("VideoRecorderManagerAction"));
        VideoRecorderManagerAction->setProperty("name", QVariant(QByteArray("VideoRecorderManagerAction")));
        widget = new QWidget(GUI);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(0, 0));
        vboxLayout = new QVBoxLayout(widget);
        vboxLayout->setSpacing(2);
        vboxLayout->setContentsMargins(5, 5, 5, 5);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        vboxLayout->setSizeConstraint(QLayout::SetNoConstraint);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        mainWidgetLayout = new QHBoxLayout();
        mainWidgetLayout->setSpacing(2);
        mainWidgetLayout->setObjectName(QStringLiteral("mainWidgetLayout"));

        vboxLayout->addLayout(mainWidgetLayout);

        GUI->setCentralWidget(widget);
        menubar = new QMenuBar(GUI);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setEnabled(true);
        menubar->setGeometry(QRect(0, 0, 800, 22));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        editMenu = new QMenu(menubar);
        editMenu->setObjectName(QStringLiteral("editMenu"));
        View = new QMenu(menubar);
        View->setObjectName(QStringLiteral("View"));
        helpMenu = new QMenu(menubar);
        helpMenu->setObjectName(QStringLiteral("helpMenu"));
        GUI->setMenuBar(menubar);
        dockWidget = new QDockWidget(GUI);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy2);
        dockWidget->setMinimumSize(QSize(200, 551));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        sizePolicy2.setHeightForWidth(dockWidgetContents->sizePolicy().hasHeightForWidth());
        dockWidgetContents->setSizePolicy(sizePolicy2);
        dockWidgetContents->setMinimumSize(QSize(200, 0));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        optionTabs = new QWidget(dockWidgetContents);
        optionTabs->setObjectName(QStringLiteral("optionTabs"));
        optionTabs->setEnabled(true);
        sizePolicy1.setHeightForWidth(optionTabs->sizePolicy().hasHeightForWidth());
        optionTabs->setSizePolicy(sizePolicy1);
        _13 = new QVBoxLayout(optionTabs);
        _13->setSpacing(2);
        _13->setContentsMargins(5, 5, 5, 5);
        _13->setObjectName(QStringLiteral("_13"));
        _13->setSizeConstraint(QLayout::SetNoConstraint);
        _14 = new QGridLayout();
        _14->setSpacing(2);
        _14->setObjectName(QStringLiteral("_14"));
        _14->setSizeConstraint(QLayout::SetNoConstraint);
        ResetViewButton = new QPushButton(optionTabs);
        ResetViewButton->setObjectName(QStringLiteral("ResetViewButton"));
        ResetViewButton->setEnabled(true);

        _14->addWidget(ResetViewButton, 2, 0, 1, 1);

        startButton = new QPushButton(optionTabs);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setCheckable(true);

        _14->addWidget(startButton, 0, 0, 1, 1);

        SaveViewButton = new QPushButton(optionTabs);
        SaveViewButton->setObjectName(QStringLiteral("SaveViewButton"));
        SaveViewButton->setEnabled(true);

        _14->addWidget(SaveViewButton, 2, 1, 1, 1);

        ResetSceneButton = new QPushButton(optionTabs);
        ResetSceneButton->setObjectName(QStringLiteral("ResetSceneButton"));
        ResetSceneButton->setEnabled(true);

        _14->addWidget(ResetSceneButton, 1, 0, 1, 1);

        stepButton = new QPushButton(optionTabs);
        stepButton->setObjectName(QStringLiteral("stepButton"));
        stepButton->setAutoRepeat(true);

        _14->addWidget(stepButton, 0, 1, 1, 1);

        screenshotButton = new QPushButton(optionTabs);
        screenshotButton->setObjectName(QStringLiteral("screenshotButton"));

        _14->addWidget(screenshotButton, 3, 0, 1, 2);

        _15 = new QHBoxLayout();
        _15->setSpacing(2);
        _15->setObjectName(QStringLiteral("_15"));
        _15->setSizeConstraint(QLayout::SetNoConstraint);
        dtLabel = new QLabel(optionTabs);
        dtLabel->setObjectName(QStringLiteral("dtLabel"));
        dtLabel->setWordWrap(false);

        _15->addWidget(dtLabel);

        dtEdit = new QLineEdit(optionTabs);
        dtEdit->setObjectName(QStringLiteral("dtEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(dtEdit->sizePolicy().hasHeightForWidth());
        dtEdit->setSizePolicy(sizePolicy3);
        dtEdit->setMinimumSize(QSize(20, 0));
        dtEdit->setMaximumSize(QSize(80, 32767));

        _15->addWidget(dtEdit);

        realTimeCheckBox = new QCheckBox(optionTabs);
        realTimeCheckBox->setObjectName(QStringLiteral("realTimeCheckBox"));

        _15->addWidget(realTimeCheckBox);


        _14->addLayout(_15, 1, 1, 1, 1);


        _13->addLayout(_14);

        spacer_tab = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Fixed);

        _13->addItem(spacer_tab);

        tabs = new QTabWidget(optionTabs);
        tabs->setObjectName(QStringLiteral("tabs"));
        sizePolicy1.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy1);
        tabs->setMinimumSize(QSize(200, 0));
        tabView = new QWidget();
        tabView->setObjectName(QStringLiteral("tabView"));
        sizePolicy1.setHeightForWidth(tabView->sizePolicy().hasHeightForWidth());
        tabView->setSizePolicy(sizePolicy1);
        gridLayout1 = new QGridLayout(tabView);
        gridLayout1->setSpacing(2);
        gridLayout1->setContentsMargins(5, 5, 5, 5);
        gridLayout1->setObjectName(QStringLiteral("gridLayout1"));
        _17 = new QGridLayout();
        _17->setSpacing(2);
        _17->setObjectName(QStringLiteral("_17"));
        _18 = new QGridLayout();
        _18->setSpacing(2);
        _18->setObjectName(QStringLiteral("_18"));
        spacer8 = new QSpacerItem(0, 16, QSizePolicy::Ignored, QSizePolicy::Minimum);

        _18->addItem(spacer8, 0, 2, 1, 1);

        pixmapLabel1 = new QLabel(tabView);
        pixmapLabel1->setObjectName(QStringLiteral("pixmapLabel1"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pixmapLabel1->sizePolicy().hasHeightForWidth());
        pixmapLabel1->setSizePolicy(sizePolicy4);
        pixmapLabel1->setPixmap(QPixmap(QString::fromUtf8("image0")));
        pixmapLabel1->setScaledContents(true);
        pixmapLabel1->setWordWrap(false);

        _18->addWidget(pixmapLabel1, 0, 1, 1, 1);

        spacer7 = new QSpacerItem(0, 16, QSizePolicy::Ignored, QSizePolicy::Minimum);

        _18->addItem(spacer7, 0, 0, 1, 1);


        _17->addLayout(_18, 1, 0, 1, 1);


        gridLayout1->addLayout(_17, 2, 0, 1, 1);

        tabs->addTab(tabView, QString());
        TabGraph = new QWidget();
        TabGraph->setObjectName(QStringLiteral("TabGraph"));
        _19 = new QVBoxLayout(TabGraph);
        _19->setSpacing(2);
        _19->setContentsMargins(5, 5, 5, 5);
        _19->setObjectName(QStringLiteral("_19"));
        ExportGraphButton = new QPushButton(TabGraph);
        ExportGraphButton->setObjectName(QStringLiteral("ExportGraphButton"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("image1"), QSize(), QIcon::Normal, QIcon::Off);
        ExportGraphButton->setIcon(icon3);

        _19->addWidget(ExportGraphButton);

        tabs->addTab(TabGraph, QString());
        TabVisualGraph = new QWidget();
        TabVisualGraph->setObjectName(QStringLiteral("TabVisualGraph"));
        _20 = new QVBoxLayout(TabVisualGraph);
        _20->setSpacing(2);
        _20->setContentsMargins(5, 5, 5, 5);
        _20->setObjectName(QStringLiteral("_20"));
        ExportVisualGraphButton = new QPushButton(TabVisualGraph);
        ExportVisualGraphButton->setObjectName(QStringLiteral("ExportVisualGraphButton"));
        ExportVisualGraphButton->setIcon(icon3);

        _20->addWidget(ExportVisualGraphButton);

        tabs->addTab(TabVisualGraph, QString());
        TabStats = new QWidget();
        TabStats->setObjectName(QStringLiteral("TabStats"));
        _21 = new QVBoxLayout(TabStats);
        _21->setSpacing(2);
        _21->setContentsMargins(5, 5, 5, 5);
        _21->setObjectName(QStringLiteral("_21"));
        dumpStateCheckBox = new QCheckBox(TabStats);
        dumpStateCheckBox->setObjectName(QStringLiteral("dumpStateCheckBox"));

        _21->addWidget(dumpStateCheckBox);

        displayComputationTimeCheckBox = new QCheckBox(TabStats);
        displayComputationTimeCheckBox->setObjectName(QStringLiteral("displayComputationTimeCheckBox"));

        _21->addWidget(displayComputationTimeCheckBox);

        exportGnuplotFilesCheckbox = new QCheckBox(TabStats);
        exportGnuplotFilesCheckbox->setObjectName(QStringLiteral("exportGnuplotFilesCheckbox"));

        _21->addWidget(exportGnuplotFilesCheckbox);

        exportVisitorCheckbox = new QCheckBox(TabStats);
        exportVisitorCheckbox->setObjectName(QStringLiteral("exportVisitorCheckbox"));

        _21->addWidget(exportVisitorCheckbox);

        tabs->addTab(TabStats, QString());
        TabPage = new QWidget();
        TabPage->setObjectName(QStringLiteral("TabPage"));
        _22 = new QVBoxLayout(TabPage);
        _22->setSpacing(2);
        _22->setContentsMargins(5, 5, 5, 5);
        _22->setObjectName(QStringLiteral("_22"));
        _23 = new QHBoxLayout();
        _23->setSpacing(0);
        _23->setObjectName(QStringLiteral("_23"));
        spacer5_3_3 = new QSpacerItem(10, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _23->addItem(spacer5_3_3);

        sizeW = new QSpinBox(TabPage);
        sizeW->setObjectName(QStringLiteral("sizeW"));
        sizeW->setMaximum(30000);
        sizeW->setValue(800);

        _23->addWidget(sizeW);

        textLabel_sizeX = new QLabel(TabPage);
        textLabel_sizeX->setObjectName(QStringLiteral("textLabel_sizeX"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(textLabel_sizeX->sizePolicy().hasHeightForWidth());
        textLabel_sizeX->setSizePolicy(sizePolicy5);
        textLabel_sizeX->setWordWrap(false);

        _23->addWidget(textLabel_sizeX);

        sizeH = new QSpinBox(TabPage);
        sizeH->setObjectName(QStringLiteral("sizeH"));
        sizeH->setMaximum(30000);
        sizeH->setValue(600);

        _23->addWidget(sizeH);

        spacer5_3_2_3 = new QSpacerItem(10, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _23->addItem(spacer5_3_2_3);


        _22->addLayout(_23);

        textEdit1 = new QTextBrowser(TabPage);
        textEdit1->setObjectName(QStringLiteral("textEdit1"));

        _22->addWidget(textEdit1);

        tabs->addTab(TabPage, QString());

        _13->addWidget(tabs);


        horizontalLayout->addWidget(optionTabs);

        dockWidget->setWidget(dockWidgetContents);
        GUI->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(editMenu->menuAction());
        menubar->addAction(View->menuAction());
        menubar->addAction(helpMenu->menuAction());
        fileMenu->addAction(fileNewAction);
        fileMenu->addAction(fileOpenAction);
        fileMenu->addAction(fileReloadAction);
        fileMenu->addAction(fileSaveAction);
        fileMenu->addAction(fileSaveAsAction);
        fileMenu->addSeparator();
        fileMenu->addSeparator();
        fileMenu->addAction(fileExitAction);
        editMenu->addAction(editRecordDirectoryAction);
        editMenu->addAction(editGnuplotDirectoryAction);
        editMenu->addSeparator();
        editMenu->addAction(PluginManagerAction);
        editMenu->addAction(MouseManagerAction);
        editMenu->addAction(VideoRecorderManagerAction);
        helpMenu->addAction(helpContentsAction);
        helpMenu->addAction(helpIndexAction);
        helpMenu->addSeparator();
        helpMenu->addAction(helpAboutAction);

        retranslateUi(GUI);
        QObject::connect(fileNewAction, SIGNAL(triggered()), GUI, SLOT(fileNew()));
        QObject::connect(fileOpenAction, SIGNAL(triggered()), GUI, SLOT(fileOpen()));
        QObject::connect(fileReloadAction, SIGNAL(triggered()), GUI, SLOT(fileReload()));
        QObject::connect(fileSaveAction, SIGNAL(triggered()), GUI, SLOT(fileSave()));
        QObject::connect(fileSaveAsAction, SIGNAL(triggered()), GUI, SLOT(fileSaveAs()));
        QObject::connect(fileExitAction, SIGNAL(triggered()), GUI, SLOT(fileExit()));
        QObject::connect(helpIndexAction, SIGNAL(triggered()), GUI, SLOT(helpIndex()));
        QObject::connect(helpContentsAction, SIGNAL(triggered()), GUI, SLOT(helpContents()));
        QObject::connect(helpAboutAction, SIGNAL(triggered()), GUI, SLOT(helpAbout()));
        QObject::connect(editRecordDirectoryAction, SIGNAL(triggered()), GUI, SLOT(editRecordDirectory()));
        QObject::connect(PluginManagerAction, SIGNAL(triggered()), GUI, SLOT(showPluginManager()));
        QObject::connect(MouseManagerAction, SIGNAL(triggered()), GUI, SLOT(showMouseManager()));
        QObject::connect(editGnuplotDirectoryAction, SIGNAL(triggered()), GUI, SLOT(editGnuplotDirectory()));
        QObject::connect(VideoRecorderManagerAction, SIGNAL(triggered()), GUI, SLOT(showVideoRecorderManager()));

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GUI);
    } // setupUi

    void retranslateUi(QMainWindow *GUI)
    {
        GUI->setWindowTitle(QApplication::translate("GUI", "Sofa", 0));
        fileNewAction->setText(QApplication::translate("GUI", "&New", 0));
        fileNewAction->setIconText(QApplication::translate("GUI", "New", 0));
        fileNewAction->setShortcut(QApplication::translate("GUI", "Ctrl+N", 0));
        fileOpenAction->setText(QApplication::translate("GUI", "&Open...", 0));
        fileOpenAction->setIconText(QApplication::translate("GUI", "Open", 0));
        fileOpenAction->setShortcut(QApplication::translate("GUI", "Ctrl+O", 0));
        fileReloadAction->setText(QApplication::translate("GUI", "&Reload", 0));
        fileReloadAction->setIconText(QApplication::translate("GUI", "Reload", 0));
        fileReloadAction->setShortcut(QApplication::translate("GUI", "Ctrl+R", 0));
        fileSaveAction->setText(QApplication::translate("GUI", "&Save", 0));
        fileSaveAction->setIconText(QApplication::translate("GUI", "Save", 0));
        fileSaveAction->setShortcut(QApplication::translate("GUI", "Ctrl+S", 0));
        fileSaveAsAction->setText(QApplication::translate("GUI", "Save &As...", 0));
        fileSaveAsAction->setIconText(QApplication::translate("GUI", "Save As", 0));
        fileSaveAsAction->setShortcut(QString());
        fileExitAction->setText(QApplication::translate("GUI", "E&xit", 0));
        fileExitAction->setIconText(QApplication::translate("GUI", "Exit", 0));
        fileExitAction->setShortcut(QString());
        helpContentsAction->setText(QApplication::translate("GUI", "&Contents...", 0));
        helpContentsAction->setIconText(QApplication::translate("GUI", "Contents", 0));
        helpContentsAction->setShortcut(QString());
        helpIndexAction->setText(QApplication::translate("GUI", "&Index...", 0));
        helpIndexAction->setIconText(QApplication::translate("GUI", "Index", 0));
        helpIndexAction->setShortcut(QString());
        helpAboutAction->setText(QApplication::translate("GUI", "&About", 0));
        helpAboutAction->setIconText(QApplication::translate("GUI", "About", 0));
        helpAboutAction->setShortcut(QString());
        ViewerAction->setIconText(QApplication::translate("GUI", "Viewer", 0));
        editRecordDirectoryAction->setText(QApplication::translate("GUI", "Record Directory...", 0));
        editRecordDirectoryAction->setIconText(QApplication::translate("GUI", "Record Directory...", 0));
        editGnuplotDirectoryAction->setText(QApplication::translate("GUI", "Gnuplot Directory...", 0));
        editGnuplotDirectoryAction->setIconText(QApplication::translate("GUI", "Gnuplot Directory...", 0));
        PluginManagerAction->setText(QApplication::translate("GUI", "Plugin Manager...", 0));
        PluginManagerAction->setIconText(QApplication::translate("GUI", "Plugin Manager...", 0));
        MouseManagerAction->setText(QApplication::translate("GUI", "Mouse Manager...", 0));
        MouseManagerAction->setIconText(QApplication::translate("GUI", "Mouse Manager...", 0));
        Action->setIconText(QApplication::translate("GUI", "Recently Opened Files...", 0));
        TestAction->setText(QApplication::translate("GUI", "Test", 0));
        TestAction->setIconText(QApplication::translate("GUI", "Test", 0));
        VideoRecorderManagerAction->setText(QApplication::translate("GUI", "Video Recorder Manager...", 0));
        VideoRecorderManagerAction->setIconText(QApplication::translate("GUI", "Video Recorder Manager...", 0));
        fileMenu->setTitle(QApplication::translate("GUI", "&File", 0));
        editMenu->setTitle(QApplication::translate("GUI", "&Edit", 0));
        View->setTitle(QApplication::translate("GUI", "&View", 0));
        helpMenu->setTitle(QApplication::translate("GUI", "Help", 0));
#ifndef QT_NO_TOOLTIP
        ResetViewButton->setToolTip(QApplication::translate("GUI", "Set the camera to initial position and orientation", 0));
#endif // QT_NO_TOOLTIP
        ResetViewButton->setText(QApplication::translate("GUI", "Reset &View", 0));
        ResetViewButton->setShortcut(QApplication::translate("GUI", "Alt+V", 0));
#ifndef QT_NO_TOOLTIP
        startButton->setToolTip(QApplication::translate("GUI", "Launch the Simulation", 0));
#endif // QT_NO_TOOLTIP
        startButton->setText(QApplication::translate("GUI", "&Animate", 0));
        startButton->setShortcut(QApplication::translate("GUI", "Alt+A", 0));
#ifndef QT_NO_TOOLTIP
        SaveViewButton->setToolTip(QApplication::translate("GUI", "Save the position and orientation of the camera", 0));
#endif // QT_NO_TOOLTIP
        SaveViewButton->setText(QApplication::translate("GUI", "Save Vie&w", 0));
        SaveViewButton->setShortcut(QApplication::translate("GUI", "Alt+W", 0));
#ifndef QT_NO_TOOLTIP
        ResetSceneButton->setToolTip(QApplication::translate("GUI", "Reset the Simulation  (t=0)", 0));
#endif // QT_NO_TOOLTIP
        ResetSceneButton->setText(QApplication::translate("GUI", "&Reset Scene", 0));
        ResetSceneButton->setShortcut(QApplication::translate("GUI", "Alt+R", 0));
#ifndef QT_NO_TOOLTIP
        stepButton->setToolTip(QApplication::translate("GUI", "Compute the simulation at time t+DT", 0));
#endif // QT_NO_TOOLTIP
        stepButton->setText(QApplication::translate("GUI", "S&tep", 0));
        stepButton->setShortcut(QApplication::translate("GUI", "Alt+T", 0));
        screenshotButton->setText(QApplication::translate("GUI", "Save S&creenshot", 0));
        screenshotButton->setShortcut(QApplication::translate("GUI", "Alt+C", 0));
        dtLabel->setText(QApplication::translate("GUI", "DT:", 0));
#ifndef QT_NO_TOOLTIP
        realTimeCheckBox->setToolTip(QApplication::translate("GUI", "Use the duration of the previous simulation step as the next DT", 0));
#endif // QT_NO_TOOLTIP
        realTimeCheckBox->setText(QApplication::translate("GUI", "Real Time", 0));
#ifndef QT_NO_TOOLTIP
        pixmapLabel1->setToolTip(QApplication::translate("GUI", "http://sofa-framework.org", 0));
#endif // QT_NO_TOOLTIP
        tabs->setTabText(tabs->indexOf(tabView), QApplication::translate("GUI", "View", 0));
        ExportGraphButton->setText(QApplication::translate("GUI", "E&xport Graph...", 0));
        ExportGraphButton->setShortcut(QApplication::translate("GUI", "Alt+X", 0));
        tabs->setTabText(tabs->indexOf(TabGraph), QApplication::translate("GUI", "Graph", 0));
        ExportVisualGraphButton->setText(QApplication::translate("GUI", "E&xport Graph...", 0));
        ExportVisualGraphButton->setShortcut(QApplication::translate("GUI", "Alt+X", 0));
        tabs->setTabText(tabs->indexOf(TabVisualGraph), QApplication::translate("GUI", "Visual", 0));
#ifndef QT_NO_TOOLTIP
        dumpStateCheckBox->setToolTip(QApplication::translate("GUI", "Record the state at each time step in file \"dumpState.data\"", "This is used to plut curves fter a simulation"));
#endif // QT_NO_TOOLTIP
        dumpStateCheckBox->setText(QApplication::translate("GUI", "Dump State", 0));
#ifndef QT_NO_TOOLTIP
        displayComputationTimeCheckBox->setToolTip(QApplication::translate("GUI", "Display information about the time spent at each branch of the graph", 0));
#endif // QT_NO_TOOLTIP
        displayComputationTimeCheckBox->setText(QApplication::translate("GUI", "Log Time", 0));
#ifndef QT_NO_TOOLTIP
        exportGnuplotFilesCheckbox->setToolTip(QApplication::translate("GUI", "Create gnuplot files for each named element of the simulation", 0));
#endif // QT_NO_TOOLTIP
        exportGnuplotFilesCheckbox->setText(QApplication::translate("GUI", "Export state as gnuplot files", 0));
#ifndef QT_NO_TOOLTIP
        exportVisitorCheckbox->setToolTip(QApplication::translate("GUI", "Open a Dialog showing a trace of the execution and time spent", 0));
#endif // QT_NO_TOOLTIP
        exportVisitorCheckbox->setText(QApplication::translate("GUI", "Trace Visitor and Component execution", 0));
        tabs->setTabText(tabs->indexOf(TabStats), QApplication::translate("GUI", "Stats", 0));
        textLabel_sizeX->setText(QApplication::translate("GUI", "<p align=\"center\">x</p>", 0));
        tabs->setTabText(tabs->indexOf(TabPage), QApplication::translate("GUI", "Viewer", 0));
    } // retranslateUi

};

namespace Ui {
    class SOFA_SOFAGUIQT_API GUI: public Ui_GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
