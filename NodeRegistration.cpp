#include "NodeRegistration.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <vector>

namespace NodeReg {
QString metadataAnalyticsLeader;
quint64 nodeUID; // Public variable to store the UID




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

void sendHeartBeat(QTcpSocket *clientSocket) {
    QJsonObject responseObj;
    responseObj["requestType"] = "Heartbeat Response";
    responseObj["message"] = "I am alive";
    responseObj["status"] = "OK";

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();

    clientSocket->write(responseData);
    clientSocket->waitForBytesWritten();
}

void handleNodeDiscovery(QTcpSocket* socket) {
    QObject::connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject message = doc.object();
        QString requestType = message["requestType"].toString();

        if (requestType == "Node Discovery") {
            QJsonArray nodesArray = message["nodes"].toArray();
            std::vector<std::string> ingestionNodes;
            for (const auto& value : nodesArray) {
                QJsonObject nodeObj = value.toObject();
                if (nodeObj["nodeType"].toString().contains("ingestion")) {
                    std::string nodeIP = nodeObj["Ip"].toString().toStdString();
                    ingestionNodes.push_back(nodeIP);
                }
            }

            metadataAnalyticsLeader = message["metadataAnalyticsLeader"].toString();
            std::cout << "Received Node Discovery update with " << ingestionNodes.size() << " nodes." << std::endl;
            std::cout << "Metadata Analytics Leader: " << metadataAnalyticsLeader.toStdString() << std::endl;
        }
        else if (requestType == "Heartbeat") {
            sendHeartBeat(socket);
        } else if (requestType == "registering") {
            nodeUID = message["Current Number"].toString().toULongLong();
            std::cout << "Received UID: " << nodeUID << std::endl;
        }
    });
}

QTcpSocket* setupRegistration(const QString& host, quint16 port, const QString& nodeType, double computingCapacity) {
    QTcpSocket* socket = registerNode(host, port, nodeType, computingCapacity);
    if (socket) {
        // Wait for response from registration
        if (socket->waitForReadyRead(5000)) {
            handleNodeDiscovery(socket);
            QByteArray data = socket->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject message = doc.object();
            if (message["requestType"].toString() == "Node Discovery") {
                metadataAnalyticsLeader = message["metadataAnalyticsLeader"].toString();
                std::cout << "Initial Metadata Analytics Leader: " << metadataAnalyticsLeader.toStdString() << std::endl;
            } else if (message["requestType"].toString() == "registering") {
                nodeUID = message["Current Number"].toString().toULongLong();
                std::cout << "Received UID: " << nodeUID << std::endl;
            }
        } else {
            std::cerr << "Failed to receive registration response." << std::endl;
            delete socket;
            return nullptr;
        }
    }
    return socket;
}

QString getMetadataAnalyticsLeader() {
    return metadataAnalyticsLeader;
}




}
