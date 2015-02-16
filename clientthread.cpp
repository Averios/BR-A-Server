#include "clientthread.h"

ClientThread::ClientThread(qintptr ID, QList<ClientThread*>* clientList, QObject *parent) :
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
    processTimer->start(100);
    Last = sf::seconds(0);
    moveQueue = new QQueue<QPair<QString, float> >();
    moveCounter = 0;
    movespeed = 150;

    qDebug() << socketDescriptor << " Socket connected";

    startGame();


    exec();
}

void ClientThread::readyRead(){
    QByteArray data = socket->readAll();
    QString stream(data);
//    QStringList stringList = stream.split(QRegExp("\n|\r\n|\r"));
//    const int listSize = stringList.size();
    for(QString now : stream.split("\n")){
        //if the character is walking then process it here
    qDebug() << stream;
        if(now == NULL)continue;
        if(now.at(0) == 'W'){
//            Elapsed = myClock.getElapsedTime() - Last;
//            Last = myClock.getElapsedTime();
            moveQueue->append(QPair<QString, float>(now, Elapsed.asSeconds()));
        }
        else if(now.at(0) == 'F'){
            QString Fire = now;
//            broadcast->addEvent(Fire);
            QString Fire2 = Fire.split("\n").at(0);
            Fire2.append(QString(" ") + QString::number(playerNumber));
            bullet->append(Fire2);
        }
        else if(now.at(0) == 'C'){
            QString message;
            QStringList msgList = now.split(" ");
            int msgSize = msgList.size();
            for(int j = 1; j < msgSize; j++){
                message += " " + msgList.at(j);
            }
            broadcast->append(QString("C ") + QString::number(this->playerNumber) + message + "\n");
        }
    }
}

void ClientThread::addBroadcaster(QQueue<QString> *broadcast){
    this->broadcast = broadcast;
}

void ClientThread::addBulletCalculator(QQueue<QString> *bullet){
    this->bullet = bullet;
}

void ClientThread::sendData(QString data){
//    qDebug() << data;
    this->socket->write(data.toUtf8());
//    this->socket->flush();
}

void ClientThread::disconnected(){
    qDebug() << "Disconnected";

    clientList->removeOne(this);
    clientMap->remove(playerNumber);
    socket->deleteLater();
    exit(0);
}

void ClientThread::processQueue(){
    //Do calculation and send to broadcaster
    Elapsed = myClock.restart();
    bool queueEmpty = moveQueue->empty();
    QPair<QString, float> moveString;
    Elapsed /= (float)moveQueue->size();
    while(!moveQueue->empty()){
        movement.x = 0;
        movement.y = 0;
        moveString = moveQueue->dequeue();
        switch(moveString.first.at(1).toLatin1()){
            case 'U':
                movement.y -= movespeed;
                break;
            case 'D':
                movement.y += movespeed;
                break;
            case 'R':
                movement.x += movespeed;
                break;
            case 'L':
                movement.x -= movespeed;
                break;
        }
        movement *= Elapsed.asSeconds();
        bool collided = false;
        for(const tmx::MapObject* obj : map->QueryQuadTree(this->getBoundingBox())){
            if(obj->GetName() == "Wall" || obj->GetName() == "Edge"){
                if(obj->GetAABB().intersects(sf::FloatRect(position.x + movement.x, position.y + movement.y + 32.f, 32.f, 16.f))){
                    collided = true;
                    break;
                }
            }
        }
        if(!collided){
            position += movement;
        }
        int moveNumber = moveString.first.split(" ").at(1).toInt();
        if(moveNumber > moveCounter){
            moveCounter = moveNumber;
        }
    }
    if(!queueEmpty){
        //W num U/D/R/L X Y Seq
        QString theEvent = moveString.first.split(" ").at(0) + QString(" ") + QString::number(playerNumber) + QString(" ") + QString::number(position.x) + QString(" ") + QString::number(position.y) + " " + QString::number(moveCounter) + "\n";
        std::cout << theEvent.toStdString();
        broadcast->append(theEvent);
    }

}

sf::FloatRect ClientThread::getBoundingBox(){
    return sf::FloatRect(position.x, position.y, 32, 48);
}

void ClientThread::setEnvironment(tmx::MapLoader *map){
    this->map = map;
}

void ClientThread::startGame(){
    //Send notification to player
    QString message("P " + QString::number(playerNumber).toUtf8() + "\n");
    qDebug() << message;
    socket->write(message.toUtf8());
//    broadcast->respawn(this->playerNumber);
//    socket->flush();
//    broadcast->StartGame();
    broadcast->append("GS\n");
}

void ClientThread::setNumber(int number){
    this->playerNumber = number;
}

ClientThread::~ClientThread(){
    delete processTimer;
    delete moveQueue;
}

int ClientThread::getNumber(){
    return playerNumber;
}

void ClientThread::setInitialPosition(sf::Vector2f position){
    this->position = position;
}

void ClientThread::resetCounter(){
    moveCounter = 0;
}

void ClientThread::setMap(QMap<int, ClientThread *> *clientMap){
    this->clientMap = clientMap;
}
