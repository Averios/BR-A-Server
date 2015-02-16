#ifndef BULLETCALCULATOR_H
#define BULLETCALCULATOR_H

#include "batchprocessor.h"
#include "clientthread.h"
#include "bullet.h"
#include "broadcaster.h"

class BulletCalculator : public BatchProcessor
{
    Q_OBJECT
public:
    explicit BulletCalculator(QList<ClientThread *> *clientList, QObject *parent = 0);
    void addEvent(QString event);
    void addBroadcaster(Broadcaster *broadcast);
signals:

public slots:
    void processQueue();

private:
    void OnStart();
    void loadMap();
    Broadcaster* broadcast;
    QList<ClientThread*>* clientList;
    QList<Bullet> BulletList;
    QList<Bullet> removal;

    sf::Clock myClock;
    sf::Time Elapsed;
    tmx::MapLoader* map;
    double bulletSpeed;
};

#endif // BULLETCALCULATOR_H
