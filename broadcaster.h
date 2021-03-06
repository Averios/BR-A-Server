#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "batchprocessor.h"
#include "clientthread.h"
#include <algorithm>
#include <random>

class ClientThread;

class Broadcaster : public BatchProcessor
{
    Q_OBJECT
public:
    explicit Broadcaster(QList<ClientThread*>* clientList, QObject *parent = 0);
    void setMap(QMap<int, ClientThread*>* clientMap);
    void addEvent(QString event);
    void respawn(int player);
    void StartGame();
    tmx::MapLoader* getMap();
signals:

public slots:
    void processQueue();

private:
    void OnStart();
    void initiatePosition();
    QList<ClientThread*>* clientList;
    QMap<int, ClientThread*>* clientMap;
    std::vector<sf::Vector2f> SpawnPoint;
    std::random_device rd;
    std::mt19937 rng;
    bool started;
};

#endif // BROADCASTER_H
