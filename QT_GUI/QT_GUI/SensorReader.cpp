#include "SensorReader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

SensorReader::SensorReader(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void SensorReader::fetchLatestSensorData() {
    QNetworkRequest request;

    QString org = "Interkoneksi";
    QString token = "kfVeQsmcnkKXFABXQr9-TJSZ3G_Wz2Xo4kmTDgFlwo-jvCTUyltw6NTqule_MOZNZdsVQfieX80g-Ycz76dcSA=";
    QString url = "http://localhost:8086/api/v2/query?org=" + org;

    request.setUrl(QUrl(url));
    request.setRawHeader("Authorization", QString("Token %1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/vnd.flux");

    QString fluxQuery = R"(
from(bucket: "ISI")
  |> range(start: -5m)
  |> filter(fn: (r) => r._measurement == "sensor_data")
  |> filter(fn: (r) => r.sensor_id == "SHT20-PascaPanen-001")
  |> filter(fn: (r) => r.location == "Gudang Fermentasi 1")
  |> filter(fn: (r) => r._field == "temperature" or r._field == "humidity")
  |> last()
)";

    QNetworkReply *reply = manager->post(request, fluxQuery.toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit sensorDataFailed("Network error: " + reply->errorString());
            reply->deleteLater();
            return;
        }

        QByteArray responseData = reply->readAll();
        qDebug() << "[SensorReader] JSON Response:" << responseData;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            emit sensorDataFailed("JSON Parse Error: " + parseError.errorString());
            reply->deleteLater();
            return;
        }

        if (!doc.isObject()) {
            emit sensorDataFailed("Unexpected JSON format.");
            reply->deleteLater();
            return;
        }

        QMap<QString, double> result;
        QJsonObject obj = doc.object();
        QJsonArray tables = obj["tables"].toArray();
        for (const QJsonValue &table : tables) {
            QJsonArray records = table.toObject()["records"].toArray();
            for (const QJsonValue &record : records) {
                QJsonObject rec = record.toObject();
                QString field = rec["_field"].toString();
                double value = rec["_value"].toDouble();
                result[field] = value;
            }
        }

        if (!result.isEmpty()) {
            emit sensorDataReceived(result);
        } else {
            emit sensorDataFailed("No data found in result.");
        }

        reply->deleteLater();
    });
}
