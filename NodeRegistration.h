#ifndef NODEREGISTRATION_H
#define NODEREGISTRATION_H

#include <QTcpSocket>

// Function to setup registration and start listening for node updates
QTcpSocket* setupRegistration(const QString& host, quint16 port, const QString& nodeType, double computingCapacity);

#endif // NODEREGISTRATION_H

