#include "FolderDataHandler.h"
#include "NodeRegistration.h"
#include "DataStreamer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>

void setupFolderDataListener(const QString &ipAddress, quint16 port, DataStreamer &streamer)
{
    QTcpServer *server = new QTcpServer();
    if (!server->listen(QHostAddress(ipAddress), port))
    {
        std::cerr << "Failed to start the server." << std::endl;
        return;
    }

    QObject::connect(server, &QTcpServer::newConnection, [server, &streamer]()
                     {
                         QTcpSocket *socket = server->nextPendingConnection();
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
                                                      streamer.enqueueDirectory(folderName);
                                                      QString metadataAnalyticsLeader = NodeReg::getMetadataAnalyticsLeader();
                                                      std::cout << "Streaming data to metadata analytics leader: "
                                                                << metadataAnalyticsLeader.toStdString() << std::endl;
                                                      // Implement the streaming logic to metadata analytics leader
                                                  }
                                                  else
                                                  {
                                                      std::cout << "Storing processed data of replica nodes." << std::endl;
                                                      // Implement the logic to store the processed data
                                                  }
                                              }
                                          });
                     });
}
