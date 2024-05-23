// main_test.cpp
#include <gtest/gtest.h>
#include "NodeRegistration.h"
#include "FolderDataHandler.h"
#include "CSVProcessor.h"
#include "QueryModule.h"
#include "DataStreamer.h"
#include <QTcpSocket>

class NodeAppTest : public ::testing::Test {
protected:
    QString host;
    quint16 port;
    QString nodeType;
    double computingCapacity;
    QTcpSocket* socket;
    QString analyticsLeaderAddress;

    void SetUp() override {
        // Initialization code here
        host = "192.168.1.102";
        port = 12351;
        nodeType = "ingestion Client";
        computingCapacity = 1.00;
        socket = nullptr;

        // Attempt to register the node
        socket = NodeReg::setupRegistration(host, port, nodeType, computingCapacity);
        if (socket) {
            // Retrieve the analytics leader address
            analyticsLeaderAddress = NodeReg::getMetadataAnalyticsLeader();
        }
    }

    void TearDown() override {
        // Cleanup code here
        if (socket) {
            socket->close();
            delete socket;
            socket = nullptr;
        }
    }
};

// Test Case 1.1: Successful Registration
TEST_F(NodeAppTest, SuccessfulRegistration) {
    ASSERT_NE(socket, nullptr);
}

// Test Case 1.2: Registration Failure
TEST_F(NodeAppTest, RegistrationFailure) {
    QString invalidHost = "invalid_host";
    QTcpSocket* invalidSocket = NodeReg::setupRegistration(invalidHost, port, nodeType, computingCapacity);
    ASSERT_EQ(invalidSocket, nullptr);
}

// Test Case 2.1: Retrieve Leader Address
TEST_F(NodeAppTest, RetrieveLeaderAddress) {
    if (socket) {
        ASSERT_FALSE(analyticsLeaderAddress.isEmpty());
    }
}

// Test Case 3.1: Start Streaming
TEST_F(NodeAppTest, StartStreaming) {
    if (!analyticsLeaderAddress.isEmpty()) {
        DataStreamer streamer(analyticsLeaderAddress, port);
        EXPECT_NO_THROW(streamer.startStreaming());
    }
}

// Test Case 4.1: Folder Data Listener Initialization
TEST_F(NodeAppTest, FolderDataListenerInitialization) {
    if (!analyticsLeaderAddress.isEmpty()) {
        DataStreamer streamer(analyticsLeaderAddress, port);
        EXPECT_NO_THROW(setupFolderDataListener(QHostAddress::Any, port, streamer));
    }
}

// Test Case 5.1: Start Querying
TEST_F(NodeAppTest, StartQuerying) {
    if (socket && !analyticsLeaderAddress.isEmpty()) {
        QueryModule queryModule(socket, analyticsLeaderAddress);
        EXPECT_NO_THROW(queryModule.startQuerying());
    }
}

// Test Case 6.1: Invalid Data Stream Start
TEST_F(NodeAppTest, InvalidDataStreamStart) {
    QString invalidAddress = "invalid_address";
    quint16 invalidPort = 0;

    DataStreamer streamer(invalidAddress, invalidPort);
    EXPECT_ANY_THROW(streamer.startStreaming());
}

// Test Case 7.1: Query Module with Invalid Socket
TEST_F(NodeAppTest, QueryModuleWithInvalidSocket) {
    QString analyticsLeaderAddress = "192.168.1.103";

    QueryModule queryModule(nullptr, analyticsLeaderAddress);
    EXPECT_ANY_THROW(queryModule.startQuerying());
}

// Test Case 8.1: Folder Data Listener with Invalid Path
TEST_F(NodeAppTest, FolderDataListenerWithInvalidPath) {
    QString analyticsLeaderAddress = "192.168.1.103";
    quint16 port = 12351;

    DataStreamer streamer(analyticsLeaderAddress, port);
    EXPECT_ANY_THROW(setupFolderDataListener(QHostAddress::LocalHost, port, streamer));
}

// Test Case 9.1: Check Data Streamer State
TEST_F(NodeAppTest, CheckDataStreamerState) {
    if (!analyticsLeaderAddress.isEmpty()) {
        DataStreamer streamer(analyticsLeaderAddress, port);
        streamer.startStreaming();
        ASSERT_TRUE(streamer.isStreaming());
    }
}

// Test Case 10.1: Check Registration Data
TEST_F(NodeAppTest, CheckRegistrationData) {
    ASSERT_EQ(host, "192.168.1.102");
    ASSERT_EQ(port, 12351);
    ASSERT_EQ(nodeType, "ingestion Client");
    ASSERT_DOUBLE_EQ(computingCapacity, 1.00);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
