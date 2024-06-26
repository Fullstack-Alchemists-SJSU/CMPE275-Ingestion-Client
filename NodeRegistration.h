#ifndef NODEREGISTRATION_H
#define NODEREGISTRATION_H

#include <QTcpSocket>
#include <QString>

#include "NodeRegistration.h"

namespace NodeReg {
QTcpSocket* setupRegistration(const QString& host, quint16 port, const QString& nodeType, double computingCapacity);
QString getMetadataAnalyticsLeader();
}




// Function to setup registration and start listening for node updates
QTcpSocket* setupRegistration(const QString& host, quint16 port, const QString& nodeType, double computingCapacity);

// Function to get the metadata analytics leader address
QString getMetadataAnalyticsLeader();

#endif // NODEREGISTRATION_H

