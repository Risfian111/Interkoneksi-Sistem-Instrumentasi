/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *labelSensorStatus;
    QLabel *labelServerStatus;
    QPushButton *btnStartStop;
    QLabel *labelTemperature;
    QLabel *labelHumidity;
    QHBoxLayout *tempLimitLayout;
    QLabel *labelTempLimit;
    QLineEdit *lineEditTempLimit;
    QHBoxLayout *humLimitLayout;
    QLabel *labelHumLimit;
    QLineEdit *lineEditHumLimit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(500, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelSensorStatus = new QLabel(centralwidget);
        labelSensorStatus->setObjectName(QString::fromUtf8("labelSensorStatus"));

        verticalLayout->addWidget(labelSensorStatus);

        labelServerStatus = new QLabel(centralwidget);
        labelServerStatus->setObjectName(QString::fromUtf8("labelServerStatus"));

        verticalLayout->addWidget(labelServerStatus);

        btnStartStop = new QPushButton(centralwidget);
        btnStartStop->setObjectName(QString::fromUtf8("btnStartStop"));

        verticalLayout->addWidget(btnStartStop);

        labelTemperature = new QLabel(centralwidget);
        labelTemperature->setObjectName(QString::fromUtf8("labelTemperature"));

        verticalLayout->addWidget(labelTemperature);

        labelHumidity = new QLabel(centralwidget);
        labelHumidity->setObjectName(QString::fromUtf8("labelHumidity"));

        verticalLayout->addWidget(labelHumidity);

        tempLimitLayout = new QHBoxLayout();
        tempLimitLayout->setObjectName(QString::fromUtf8("tempLimitLayout"));
        labelTempLimit = new QLabel(centralwidget);
        labelTempLimit->setObjectName(QString::fromUtf8("labelTempLimit"));

        tempLimitLayout->addWidget(labelTempLimit);

        lineEditTempLimit = new QLineEdit(centralwidget);
        lineEditTempLimit->setObjectName(QString::fromUtf8("lineEditTempLimit"));

        tempLimitLayout->addWidget(lineEditTempLimit);


        verticalLayout->addLayout(tempLimitLayout);

        humLimitLayout = new QHBoxLayout();
        humLimitLayout->setObjectName(QString::fromUtf8("humLimitLayout"));
        labelHumLimit = new QLabel(centralwidget);
        labelHumLimit->setObjectName(QString::fromUtf8("labelHumLimit"));

        humLimitLayout->addWidget(labelHumLimit);

        lineEditHumLimit = new QLineEdit(centralwidget);
        lineEditHumLimit->setObjectName(QString::fromUtf8("lineEditHumLimit"));

        humLimitLayout->addWidget(lineEditHumLimit);


        verticalLayout->addLayout(humLimitLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Smart Agriculture Monitor", nullptr));
        labelSensorStatus->setText(QCoreApplication::translate("MainWindow", "Sensor: \342\235\214 Disconnected", nullptr));
        labelServerStatus->setText(QCoreApplication::translate("MainWindow", "Server: \342\235\214 Disconnected", nullptr));
        btnStartStop->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        labelTemperature->setText(QCoreApplication::translate("MainWindow", "Suhu: -- \302\260C", nullptr));
        labelHumidity->setText(QCoreApplication::translate("MainWindow", "Kelembaban: -- %", nullptr));
        labelTempLimit->setText(QCoreApplication::translate("MainWindow", "Batas Suhu:", nullptr));
        labelHumLimit->setText(QCoreApplication::translate("MainWindow", "Batas Kelembaban:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
