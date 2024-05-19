#include "FolderDataHandler.h"
#include "NodeRegistration.h"
#include "DataStreamer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

void setupFolderDataListener(QTcpSocket *socket, DataStreamer &streamer)
{
    QObject::connect(socket, &QTcpSocket::readyRead, [socket, &streamer]()
                     {
        QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject message = doc.object();
        if (message.contains("Folder name") && message.contains("isPrimary")) {
            QString folderName = message["Folder name"].toString();
            bool isPrimary = message["isPrimary"].toBool();
            std::cout << "Received folder data: " << folderName.toStdString()
                      << ", isPrimary: " << (isPrimary ? "true" : "false") << std::endl;

            // Process the folder data
            if (isPrimary)
            {
                /*TODO
                 * FolderDataHandler.cpp:30:9: Use of undeclared identifier 'streamer'
                    FolderDataHandler.cpp:24:17: Variable 'streamer' cannot be implicitly captured in a lambda with no capture-default specified (fix available)
                    FolderDataHandler.cpp:8:64: 'streamer' declared here
                    FolderDataHandler.cpp:10:54: lambda expression begins here
                 * */
                [&streamer, folderName]() {
                    streamer.enqueueDirectory(folderName);
                };
                QString metadataAnalyticsLeader = NodeReg::getMetadataAnalyticsLeader();
                std::cout << "Streaming data to metadata analytics leader: "
                          << metadataAnalyticsLeader.toStdString() << std::endl;
                // Implement the streaming logic to metadata analytics leader
                // Do we need to handle this here
            }
            else
            {
                std::cout << "Storing processed data of replica nodes." << std::endl;
                // Implement the logic to store the processed data
            }
        }

    });
}

