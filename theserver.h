#ifndef THESERVER_H
#define THESERVER_H

#include <QTcpServer>
#include "broadcaster.h"
#include "clientthread.h"

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
    QLinkedList<ClientThread*> clientList;
    Broadcaster* broadcast;
    int playerCount;
};

#endif // THESERVER_H
