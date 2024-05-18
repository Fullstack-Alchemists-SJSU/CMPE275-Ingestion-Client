#include "NodeRegistration.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

QTcpSocket* registerNode(const QString& host, quint16 port, const QString& nodeType, double computingCapacity) {
    auto socket = new QTcpSocket();
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(5000)) {
        std::cerr << "Failed to connect to the registration server." << std::endl;
        delete socket; // Ensure socket is deleted if connection fails
        return nullptr;
    }

    QJsonObject obj;
    obj["requestType"] = "registering";
    obj["Ip"] = host;
    obj["nodeType"] = nodeType;
    obj["computingCapacity"] = computingCapacity;

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    socket->write(data);
    if (!socket->waitForBytesWritten(5000)) {
        std::cerr << "Failed to write data to server." << std::endl;
        delete socket;
        return nullptr;
    }

    return socket;
}

void handleNodeDiscovery(QTcpSocket* socket) {
    QObject::connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject message = doc.object();
        if (message["requestType"].toString() == "Node Discovery") {
            QJsonArray nodesArray = message["nodes"].toArray();
            std::vector<std::string> ingestionNodes;
            for (const auto& value : nodesArray) {
                QJsonObject nodeObj = value.toObject();
                if (nodeObj["nodeType"].toString().contains("ingestion")) {
                    std::string nodeIP = nodeObj["Ip"].toString().toStdString();
                    ingestionNodes.push_back(nodeIP);
                }
            }
            std::cout << "Received Node Discovery update with " << ingestionNodes.size() << " nodes." << std::endl;
        }
    });
}

QTcpSocket* setupRegistration(const QString& host, quint16 port, const QString& nodeType, double computingCapacity) {
    QTcpSocket* socket = registerNode(host, port, nodeType, computingCapacity);
    if (socket) {
        handleNodeDiscovery(socket);
    }
    return socket;
}
