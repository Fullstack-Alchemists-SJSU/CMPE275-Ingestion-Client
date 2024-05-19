#include "NodeRegistration.h"
#include "FolderDataHandler.h"
#include "CSVProcessor.h"
#include "QueryModule.h"
#include <QCoreApplication>
#include <iostream>

using std::string;

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString host = "192.168.1.102";
    quint16 port = 12351;
    QString nodeType = "ingestion Client";
    double computingCapacity = 1.00;

    QTcpSocket* socket = NodeReg::setupRegistration(host, port, nodeType, computingCapacity);
    if (socket) {
        std::cout << "Node registered successfully and listening for updates." << std::endl;

        QString leaderAddress = NodeReg::getMetadataAnalyticsLeader();
        std::cout << "Metadata Analytics Leader Address: " << leaderAddress.toStdString() << std::endl;
        /*
        setupFolderDataListener(socket);

        string inputFile = "../sample.csv";
        string outputFile = "../processed_airnow_data.csv";
        CSVProcessor processor(inputFile, outputFile);
        processor.processFile();  // Assuming processFile() has no return or handling inside

        std::cout << "Processing complete. Output saved to: " << outputFile << std::endl;*/

        std::cout << "Processing complete. Output saved to: "<< std::endl;

        // Create and start the Query module
        QueryModule queryModule(socket, leaderAddress);
        queryModule.startQuerying();
    } else {
        std::cerr << "Failed to register the node." << std::endl;
        return -1; // Exit if registration fails
    }

    return app.exec(); // Keep the application running and listening
}
