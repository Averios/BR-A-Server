#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "batchprocessor.h"
#include "clientthread.h"

class ClientThread;

class Broadcaster : public BatchProcessor
{
    Q_OBJECT
public:
    explicit Broadcaster(QLinkedList<ClientThread*>* clientList, QObject *parent = 0);
    void addEvent(QString event);
    tmx::MapLoader* getMap();
signals:

public slots:
    void processQueue();

private:
    void OnStart();
    QLinkedList<ClientThread*>* clientList;
};

#endif // BROADCASTER_H
