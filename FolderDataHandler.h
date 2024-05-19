#ifndef FOLDERDATAHANDLER_H
#define FOLDERDATAHANDLER_H

#include <QTcpSocket>
#include <QString>
#include "DataStreamer.h"

// Function to setup listening for folder data updates
void setupFolderDataListener(QTcpSocket* socket, DataStreamer &streamer);



#endif // FOLDERDATAHANDLER_H
