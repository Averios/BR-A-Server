#include "theserver.h"

TheServer::TheServer(QObject *parent) :
    QTcpServer(parent)
{
}

void TheServer::startserver(){
    int port = 7080;

    if(!this->listen(QHostAddress::Any, port)){
        qDebug() << "Could not start server";
    }
    else{
        qDebug() << "Listening to port " << port << "...";
    }

    playerCount = 0;

    broadcast = new Broadcaster(&clientList, this);
    broadcast->run();
    bullet = new BulletCalculator(&clientList, this);
    bullet->run();
}

void TheServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "Connecting...";
    ClientThread* client = new ClientThread(socketDescriptor, &clientList, this);
    client->addBroadcaster(broadcast);
    client->addBulletCalculator(bullet);
    client->setNumber(playerCount++);
    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    client->run();
//    qDebug() << "Client Running";
//    broadcast->StartGame();
}
