#ifndef FOLDERDATAHANDLER_H
#define FOLDERDATAHANDLER_H

#include "DataStreamer.h"
#include <QTcpServer>
#include <QString>

void setupFolderDataListener(const QHostAddress &address, quint16 port, DataStreamer &streamer);

#endif // FOLDERDATAHANDLER_H
