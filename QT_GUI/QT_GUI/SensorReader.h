#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>
#include <QMap>

class QNetworkAccessManager;

class SensorReader : public QObject
{
    Q_OBJECT
public:
    explicit SensorReader(QObject *parent = nullptr);
    void fetchLatestSensorData();

signals:
    void sensorDataReceived(const QMap<QString, double> &data);
    void sensorDataFailed(const QString &error);

private:
    QNetworkAccessManager *manager;
};

#endif // SENSORREADER_H
