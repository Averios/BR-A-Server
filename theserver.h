#ifndef THESERVER_H
#define THESERVER_H

#include <QTcpServer>
#include "broadcaster.h"
#include "clientthread.h"
#include "bulletcalculator.h"

class TheServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TheServer(QObject *parent = 0);
    void startserver();
signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QList<ClientThread*> clientList;
    QMap<int, ClientThread*> clientMap;
    Broadcaster* broadcast;
    BulletCalculator* bullet;
    int playerCount;
};

#endif // THESERVER_H
