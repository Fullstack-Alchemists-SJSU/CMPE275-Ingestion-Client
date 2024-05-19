#ifndef QUERYMODULE_H
#define QUERYMODULE_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>

class QueryModule : public QObject {
    Q_OBJECT
public:
    QueryModule(QTcpSocket* sharedSocket, const QString& leaderAddress, QObject* parent = nullptr);
    void startQuerying();

private slots:
    void sendQuery();

private:
    QTcpSocket* socket;
    QTimer* timer;
    QString leaderAddress;
};

#endif // QUERYMODULE_H
