#include "QueryModule.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>

QueryModule::QueryModule(QTcpSocket* sharedSocket, const QString& leaderAddress, QObject* parent)
    : QObject(parent), socket(sharedSocket), leaderAddress(leaderAddress) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QueryModule::sendQuery);
}

void QueryModule::startQuerying() {
    timer->start(90000); // 1.5 minutes in milliseconds
}

void QueryModule::sendQuery() {
    QJsonObject queryRequest;
    queryRequest["requestType"] = "query";
    queryRequest["param"] = QJsonArray::fromVariantList(QVariantList() << QVariantList() << 0); // Example with 0, can be 0 or 1

    QJsonDocument doc(queryRequest);
    QByteArray data = doc.toJson();

    if (socket->state() == QAbstractSocket::ConnectedState || socket->waitForConnected(5000)) {
        socket->write(data);
        if (socket->waitForBytesWritten()) {
            std::cout << "Query sent to metadata analytics leader: " << leaderAddress.toStdString() << std::endl;
        } else {
            std::cerr << "Failed to write data to metadata analytics leader." << std::endl;
        }
    } else {
        std::cerr << "Failed to connect to metadata analytics leader for query." << std::endl;
    }
}
