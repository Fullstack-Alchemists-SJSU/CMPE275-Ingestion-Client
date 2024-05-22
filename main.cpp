#include "NodeRegistration.h"
#include "FolderDataHandler.h"
#include "CSVProcessor.h"
#include "QueryModule.h"
#include "DataStreamer.h"
#include <QCoreApplication>
#include <iostream>

using std::string;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString host = "192.168.1.102";
    quint16 port = 12351;
    QString nodeType = "ingestion Client";
    double computingCapacity = 1.00;

    QTcpSocket *socket = NodeReg::setupRegistration(host, port, nodeType, computingCapacity);
    if (socket)
    {
        std::cout << "Node registered successfully and listening for updates." << std::endl;

        QString analyticsLeaderAddress = NodeReg::getMetadataAnalyticsLeader();
        std::cout << "Metadata Analytics Leader Address: " << analyticsLeaderAddress.toStdString() << std::endl;


        // Initialize the Streaming to the Analytics Leader
        DataStreamer streamer(analyticsLeaderAddress, port);
        streamer.startStreaming();

        setupFolderDataListener(analyticsLeaderAddress, port, streamer);

        std::cout << "Processing complete. Output saved to: " << std::endl;

        // Create and start the Query module
        QueryModule queryModule(socket, analyticsLeaderAddress);
        queryModule.startQuerying();
    }
    else
    {
        std::cerr << "Failed to register the node." << std::endl;
        return -1; // Exit if registration fails
    }

    return app.exec(); // Keep the application running and listening
}
