#ifndef BULLETCALCULATOR_H
#define BULLETCALCULATOR_H

#include "batchprocessor.h"
#include "broadcaster.h"
#include "clientthread.h"

class ClientThread;
class Broadcaster;

class BulletCalculator : public BatchProcessor
{
    Q_OBJECT
public:
    explicit BulletCalculator(QLinkedList<ClientThread *>* clientList, QObject *parent = 0);
    void addEvent(QString event);
    void addBroadcaster(Broadcaster* broadcast);
signals:

public slots:
    void processQueue();

private:
    void OnStart();
    void loadMap();
    Broadcaster* broadcast;
    QLinkedList<ClientThread*>* clientList;

    sf::Clock myClock;
    sf::Time Elapsed;
    tmx::MapLoader* map;
};

#endif // BULLETCALCULATOR_H
