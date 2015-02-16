#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QList>
#include <QQueue>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <SFML/System.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <tmx/MapLoader.h>


class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(qintptr ID, QList<ClientThread*>* clientList, QObject *parent = 0);
    virtual ~ClientThread();
    void run();
    void addBroadcaster(QQueue<QString>* broadcast);
    void addBulletCalculator(QQueue<QString>* bullet);
    void sendData(QString data);
    void setInitialPosition(sf::Vector2f position);
    void setNumber(int number);
    void startGame();
    sf::FloatRect getBoundingBox();
    int getNumber();
    void resetCounter();
    void setMap(QMap<int, ClientThread*>* clientMap);
    void setEnvironment(tmx::MapLoader* map);

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();
    void processQueue();

private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QList<ClientThread*>* clientList;
    QMap<int, ClientThread*>* clientMap;
    QQueue<QPair<QString, float > >* moveQueue;
    QTimer* processTimer;
    QString name;

    int playerNumber;

    sf::Vector2f position;
    sf::Vector2f movement;

    QQueue<QString>* broadcast;
    QQueue<QString>* bullet;
    sf::Time Elapsed;
    sf::Time Last;
    sf::Clock myClock;
    int moveCounter;
    float movespeed;

    tmx::MapLoader* map;
};

#endif // CLIENTTHREAD_H
