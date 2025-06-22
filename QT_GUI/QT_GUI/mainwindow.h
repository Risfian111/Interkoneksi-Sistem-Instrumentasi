#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMap>

#include "SensorReader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onRefreshTimeout();
    void updateSensorData(const QMap<QString, double> &data);
    void handleSensorError(const QString &error);

private:
    Ui::MainWindow *ui;
    SensorReader *sensorReader;
    QTimer *autoRefreshTimer;
};

#endif // MAINWINDOW_H
