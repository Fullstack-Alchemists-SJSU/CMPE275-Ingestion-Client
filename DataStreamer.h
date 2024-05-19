#ifndef DATASTREAMER_H
#define DATASTREAMER_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include "CSVProcessor.h"

class DataStreamer : public QObject
{
    Q_OBJECT

public:
    explicit DataStreamer(const QString &host, quint16 port);
    void enqueueDirectory(const QString &directory);
    void startStreaming();

private slots:
    void processNextDirectory();
    void sendData();

private:
    QQueue<QString> directories;
    CSVProcessor processor;
    QTcpSocket socket;
    QTimer timer;
    QVector<QJsonDocument> dataChunks;
    int currentChunkIndex;
};

#endif // DATASTREAMER_H
