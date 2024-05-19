#include "FolderDataHandler.h"
#include "NodeRegistration.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>



void setupFolderDataListener(QTcpSocket* socket) {
    QObject::connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject message = doc.object();
        if (message.contains("Folder name") && message.contains("isPrimary")) {
            QString folderName = message["Folder name"].toString();
            bool isPrimary = message["isPrimary"].toBool();
            processFolderData(folderName, isPrimary);
        }
    });
}

void processFolderData(const QString& folderName, bool isPrimary) {
    std::cout << "Received folder data: " << folderName.toStdString()
              << ", isPrimary: " << (isPrimary ? "true" : "false") << std::endl;

    // Process the folder data
    if (isPrimary) {
        QString metadataAnalyticsLeader = NodeReg::getMetadataAnalyticsLeader();
        std::cout << "Streaming data to metadata analytics leader: "
                  << metadataAnalyticsLeader.toStdString() << std::endl;
        // Implement the streaming logic to metadata analytics leader
    } else {
        std::cout << "Storing processed data." << std::endl;
        // Implement the logic to store the processed data
    }
}
