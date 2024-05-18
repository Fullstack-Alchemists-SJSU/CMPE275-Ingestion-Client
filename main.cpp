#include "NodeRegistration.h"
#include "FolderDataHandler.h""
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString host = "registration.server.com"; //Add Registration Node IP address
    quint16 port = 12351;
    QString nodeType = "ingestion Client";
    double computingCapacity = 1.00;

    QTcpSocket* socket = setupRegistration(host, port, nodeType, computingCapacity);
    if (socket) {
        std::cout << "Node registered successfully and listening for updates." << std::endl;

        // Example usage of the metadata analytics leader address
        QString leaderAddress = getMetadataAnalyticsLeader();
        std::cout << "Metadata Analytics Leader Address: " << leaderAddress.toStdString() << std::endl;

        // Setup folder data listener
        setupFolderDataListener(socket);
    } else {
        std::cerr << "Failed to register the node." << std::endl;
        return -1; // Exit if registration fails
    }

    return app.exec(); // Keep the application running and listening
}
