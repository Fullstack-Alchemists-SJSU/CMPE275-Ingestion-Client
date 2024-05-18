#ifndef FOLDERDATAHANDLER_H
#define FOLDERDATAHANDLER_H

#include <QTcpSocket>
#include <QString>

// Function to setup listening for folder data updates
void setupFolderDataListener(QTcpSocket* socket);

// Function to process the folder data
void processFolderData(const QString& folderName, bool isPrimary);

#endif // FOLDERDATAHANDLER_H
