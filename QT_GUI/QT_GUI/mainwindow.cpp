#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sensorReader(new SensorReader(this))
    , autoRefreshTimer(new QTimer(this))
{
    ui->setupUi(this);

    ui->labelSensorStatus->setText("Sensor: ❌ Disconnected");
    ui->labelServerStatus->setText("Server: ❌ Disconnected");
    ui->labelTemperature->setText("Suhu: -- °C");
    ui->labelHumidity->setText("Kelembaban: -- %");

    connect(ui->btnStartStop, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(sensorReader, &SensorReader::sensorDataReceived, this, &MainWindow::updateSensorData);
    connect(sensorReader, &SensorReader::sensorDataFailed, this, &MainWindow::handleSensorError);
    connect(autoRefreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshTimeout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartClicked()
{
    ui->labelServerStatus->setText("Server: ⏳ Connecting...");
    sensorReader->fetchLatestSensorData();
    autoRefreshTimer->start(5000); // refresh tiap 5 detik
}

void MainWindow::onRefreshTimeout()
{
    sensorReader->fetchLatestSensorData();
}

void MainWindow::updateSensorData(const QMap<QString, double> &data)
{
    ui->labelServerStatus->setText("Server: ✅ Connected");
    ui->labelSensorStatus->setText("Sensor: ✅ Connected");

    double suhu = data.value("temperature", 0.0);
    double kelembaban = data.value("humidity", 0.0);

    ui->labelTemperature->setText(QString("Suhu: %1 °C").arg(QString::number(suhu, 'f', 1)));
    ui->labelHumidity->setText(QString("Kelembaban: %1 %").arg(QString::number(kelembaban, 'f', 1)));

    // Ambil batas dari input
    bool suhuValid = false, kelembabanValid = false;
    double batasSuhu = ui->lineEditTempLimit->text().toDouble(&suhuValid);
    double batasKelembaban = ui->lineEditHumLimit->text().toDouble(&kelembabanValid);

    // Validasi suhu
    if (suhuValid && suhu > batasSuhu) {
        ui->labelTemperature->setStyleSheet("color: red;");
    } else {
        ui->labelTemperature->setStyleSheet("color: black;");
    }

    // Validasi kelembaban
    if (kelembabanValid && kelembaban > batasKelembaban) {
        ui->labelHumidity->setStyleSheet("color: red;");
    } else {
        ui->labelHumidity->setStyleSheet("color: black;");
    }
}

void MainWindow::handleSensorError(const QString &error)
{
    qDebug() << "[MainWindow] Sensor error:" << error;
    ui->labelServerStatus->setText("Server: ❌ Error");
    ui->labelSensorStatus->setText("Sensor: ⚠️ Error");
    ui->labelTemperature->setText("Suhu: -- °C");
    ui->labelHumidity->setText("Kelembaban: -- %");
    ui->labelTemperature->setStyleSheet("color: gray;");
    ui->labelHumidity->setStyleSheet("color: gray;");
}
