#include "NodeRegistration/NodeRegistration.h"
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString host = "registration.server.com";
    quint16 port = 12345;
    QString nodeType = "ingestion_client";
    double computingCapacity = 1;

    QTcpSocket* socket = setupRegistration(host, port, nodeType, computingCapacity);
    if (socket) {
        std::cout << "Node registered successfully and listening for updates." << std::endl;
    } else {
        std::cerr << "Failed to register the node." << std::endl;
    }

    return app.exec();
}

