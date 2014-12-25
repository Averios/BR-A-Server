#include "clientthread.h"

ClientThread::ClientThread(qintptr ID, QLinkedList<ClientThread*>* clientList, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
    this->clientList = clientList;
}

void ClientThread::run(){
    qDebug() << "Thread started";

    this->socket = new QTcpSocket(this);

    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        emit error(socket->error());
        return;
    }

    clientList->append(this);
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
        //if the character is walking then process it here
        if(stringList.at(i).at(0) == 'W'){
            moveQueue->append(stringList.at(i));
        }
    }
}

void ClientThread::addBroadcaster(Broadcaster *broadcast){
    this->broadcast = broadcast;
}

void ClientThread::addBulletCalculator(BulletCalculator *bullet){
    this->bullet = bullet;
}

void ClientThread::sendData(QString data){
    this->socket->write(data.toUtf8());
}

void ClientThread::disconnected(){
    qDebug() << "Disconnected";

    clientList->removeOne(this);
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
