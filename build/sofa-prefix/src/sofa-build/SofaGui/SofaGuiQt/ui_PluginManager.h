/********************************************************************************
** Form generated from reading UI file 'PluginManager.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLUGINMANAGER_H
#define UI_PLUGINMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PluginManager
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QSplitter *unnamed;
    QTreeWidget *listPlugins;
    QWidget *layout24;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QLabel *description_label;
    QTextEdit *description;
    QTreeWidget *listComponents;
    QHBoxLayout *hboxLayout1;
    QPushButton *buttonAdd;
    QPushButton *buttonRemove;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonClose;

    void setupUi(QDialog *PluginManager)
    {
        if (PluginManager->objectName().isEmpty())
            PluginManager->setObjectName(QStringLiteral("PluginManager"));
        PluginManager->setEnabled(true);
        PluginManager->resize(800, 400);
        PluginManager->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(PluginManager);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        unnamed = new QSplitter(PluginManager);
        unnamed->setObjectName(QStringLiteral("unnamed"));
        unnamed->setOrientation(Qt::Vertical);
        listPlugins = new QTreeWidget(unnamed);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        listPlugins->setHeaderItem(__qtreewidgetitem);
        listPlugins->setObjectName(QStringLiteral("listPlugins"));
        listPlugins->setMinimumSize(QSize(416, 0));
        listPlugins->setMidLineWidth(1);
        listPlugins->setAllColumnsShowFocus(true);
        listPlugins->setProperty("showSortIndicator", QVariant(true));
        listPlugins->setProperty("itemMargin", QVariant(4));
        unnamed->addWidget(listPlugins);
        layout24 = new QWidget(unnamed);
        layout24->setObjectName(QStringLiteral("layout24"));
        hboxLayout = new QHBoxLayout(layout24);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QStringLiteral("vboxLayout1"));
        description_label = new QLabel(layout24);
        description_label->setObjectName(QStringLiteral("description_label"));
        description_label->setWordWrap(false);

        vboxLayout1->addWidget(description_label);

        description = new QTextEdit(layout24);
        description->setObjectName(QStringLiteral("description"));
        description->setEnabled(true);
        description->setUndoRedoEnabled(false);
        description->setReadOnly(true);

        vboxLayout1->addWidget(description);


        hboxLayout->addLayout(vboxLayout1);

        listComponents = new QTreeWidget(layout24);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        listComponents->setHeaderItem(__qtreewidgetitem1);
        listComponents->setObjectName(QStringLiteral("listComponents"));
        listComponents->setEnabled(true);

        hboxLayout->addWidget(listComponents);

        unnamed->addWidget(layout24);

        vboxLayout->addWidget(unnamed);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QStringLiteral("hboxLayout1"));
        buttonAdd = new QPushButton(PluginManager);
        buttonAdd->setObjectName(QStringLiteral("buttonAdd"));
        buttonAdd->setAutoDefault(true);
        buttonAdd->setDefault(true);

        hboxLayout1->addWidget(buttonAdd);

        buttonRemove = new QPushButton(PluginManager);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        buttonRemove->setAutoDefault(true);

        hboxLayout1->addWidget(buttonRemove);

        Horizontal_Spacing2 = new QSpacerItem(237, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(Horizontal_Spacing2);

        buttonClose = new QPushButton(PluginManager);
        buttonClose->setObjectName(QStringLiteral("buttonClose"));
        buttonClose->setAutoDefault(true);

        hboxLayout1->addWidget(buttonClose);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);


        retranslateUi(PluginManager);
        QObject::connect(buttonClose, SIGNAL(clicked()), PluginManager, SLOT(close()));

        QMetaObject::connectSlotsByName(PluginManager);
    } // setupUi

    void retranslateUi(QDialog *PluginManager)
    {
        PluginManager->setWindowTitle(QApplication::translate("PluginManager", "Plugin Manager", 0));
        description_label->setText(QApplication::translate("PluginManager", "Description", 0));
        buttonAdd->setText(QApplication::translate("PluginManager", "&Add...", 0));
        buttonAdd->setShortcut(QApplication::translate("PluginManager", "Alt+A", 0));
        buttonRemove->setText(QApplication::translate("PluginManager", "&Remove", 0));
        buttonRemove->setShortcut(QApplication::translate("PluginManager", "Alt+R", 0));
        buttonClose->setText(QApplication::translate("PluginManager", "&Close", 0));
        buttonClose->setShortcut(QApplication::translate("PluginManager", "Alt+C", 0));
    } // retranslateUi

};

namespace Ui {
    class PluginManager: public Ui_PluginManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLUGINMANAGER_H
