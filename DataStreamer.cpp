#include "DataStreamer.h"
#include <QDebug>

DataStreamer::DataStreamer(const QString &host, quint16 port)
{
    socket.connectToHost(host, port);
    connect(&processTimer, &QTimer::timeout, this, &DataStreamer::processNextDirectory);
    connect(&sendTimer, &QTimer::timeout, this, &DataStreamer::sendData);
}

void DataStreamer::enqueueDirectory(const QString &directory)
{
    directories.enqueue(directory);
    if (!processTimer.isActive())
    {
        processTimer.start(1000); // Start processing immediately if not already running
    }
}

void DataStreamer::startStreaming()
{
    processTimer.start(1000); // Check every second if there is a directory to process
}

void DataStreamer::processNextDirectory()
{
    if (!directories.isEmpty())
    {
        QString directory = directories.dequeue();
        CSVProcessor processor(directory);
        dataChunks = processor.processFiles(); // Process all files in the directory
        currentChunkIndex = 0;

        if (!sendTimer.isActive())
        {
            sendTimer.start(1000); // Start sending data if not already started
        }
    }
    else
    {
        processTimer.stop(); // Stop the processing timer if no more directories are left
    }
}

void DataStreamer::sendData()
{
    if (currentChunkIndex < dataChunks.size())
    {
        QJsonDocument &doc = dataChunks[currentChunkIndex++];
        QByteArray data = doc.toJson();
        socket.write(data);
        qDebug() << "Sending data chunk:" << data;
    }
    else
    {
        qDebug() << "No more data to send.";
        sendTimer.stop(); // Stop the sending timer if no more data is left to send
    }
}
