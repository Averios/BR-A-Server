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

    broadcast = new Broadcaster(&clientList, this);
    broadcast->run();
}

void TheServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "Connecting...";
    ClientThread* client = new ClientThread(socketDescriptor, &clientList, this);
    client->addBroadcaster(broadcast);
    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    client->run();
}
