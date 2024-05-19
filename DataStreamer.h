#ifndef DATASTREAMER_H
#define DATASTREAMER_H

#include <QObject>
#include <QString>
#include <QQueue>
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
    std::unique_ptr <CSVProcessor> processor;
    QTcpSocket socket;
    QTimer timer;
    QVector<QJsonDocument> dataChunks;
    int currentChunkIndex;
    QTimer processTimer;
    QTimer sendTimer;
};

#endif // DATASTREAMER_H
