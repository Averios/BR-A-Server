#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QLinkedList>
#include <QQueue>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <SFML/System.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "batchprocessor.h"
#include "broadcaster.h"
#include "bulletcalculator.h"


class BatchProcessor;
class BulletCalculator;
class Broadcaster;

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(qintptr ID, QLinkedList<ClientThread*>* clientList, QObject *parent = 0);
    virtual ~ClientThread();
    void run();
    void addBroadcaster(Broadcaster* broadcast);
    void addBulletCalculator(BulletCalculator* bullet);
    void sendData(QString data);
    void setInitialPosition(sf::Vector2f position);
    void setNumber(int number);
    void startGame();
    sf::FloatRect getBoundingBox();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();
    void processQueue();

private:
    void loadMap();
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QLinkedList<ClientThread*>* clientList;
    QQueue<QString>* moveQueue;
    QTimer* processTimer;
    QString name;

    int playerNumber;

    sf::Vector2f position;
    sf::Vector2f movement;

    Broadcaster* broadcast;
    BulletCalculator* bullet;
    sf::Time Elapsed;
    sf::Clock myClock;
    int moveCounter;
    float movespeed;

    tmx::MapLoader* map;
    QLinkedList<sf::Vector2f> playerCollisionPoint;
};

#endif // CLIENTTHREAD_H
