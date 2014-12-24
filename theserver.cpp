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
}

void TheServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "Connecting...";

}
