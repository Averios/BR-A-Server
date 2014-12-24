#include "clientthread.h"

ClientThread::ClientThread(qintptr ID, QLinkedList<ClientThread*>* clientList, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
    this->clientList = clientList;
    clientList->append(this);
}

void ClientThread::run(){
    qDebug() << "Thread started";

    this->socket = new QTcpSocket(this);

    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        emit error(socket->error());
        clientList->removeOne(this);
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    processTimer = new QTimer(this);
    connect(processTimer, SIGNAL(timeout()), this, SLOT(processQueue()));
    processTimer->start(10);

    moveQueue = new QQueue;

    qDebug() << socketDescriptor << " Socket connected";

    exec();
}

void ClientThread::readyRead(){
    QByteArray data = socket->readAll();
    QString stream(data);
    QStringList stringList = stream.split(QRegExp("\n|\r\n|\r"));
    const int listSize = stringList.size();
    for(int i = 0; i < listSize; i++){
        if(stringList.at(i).at(0) == 'W'){
            moveQueue->append(stringList.at(i));
        }
    }
}

void ClientThread::disconnected(){
    qDebug() << "Disconnected";

    socket->deleteLater();
    exit(0);
}

void ClientThread::processQueue(){
    //Do calculation and send to broadcaster
}

ClientThread::~ClientThread(){
    delete processTimer;
    delete moveQueue;
}
