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
    processTimer->start(10);
    Last = sf::seconds(0);
    moveQueue = new QQueue<QPair<QString, float> >();
    moveCounter = 0;
    movespeed = 100;

    qDebug() << socketDescriptor << " Socket connected";

    playerCollisionPoint.append(sf::Vector2f(0.f, 48.f));
    playerCollisionPoint.append(sf::Vector2f(32.f, 48.f));
    playerCollisionPoint.append(sf::Vector2f(0.f, 32.f));
    playerCollisionPoint.append(sf::Vector2f(32.f, 32.f));

    startGame();

    exec();
}

void ClientThread::readyRead(){
    QByteArray data = socket->readAll();
    QString stream(data);
//    QStringList stringList = stream.split(QRegExp("\n|\r\n|\r"));
//    const int listSize = stringList.size();
//    for(int i = 0; i < listSize; i++){
        //if the character is walking then process it here
        if(stream.at(0) == 'W'){
            Elapsed = myClock.getElapsedTime() - Last;
            Last = myClock.getElapsedTime();
            moveQueue->append(QPair<QString, float>(stream.at(0), Elapsed.asSeconds()));
        }
        else if(stream.at(0) == 'F'){
            QString Fire = stream;
            broadcast->addEvent(Fire);
            Fire.append(QString(" ") + QString::number(playerNumber));
            bullet->addEvent(Fire);
        }
        else if(stream.at(0) == 'C'){
            QString message;
            QStringList msgList = stream.split(" ");
            int msgSize = msgList.size();
            for(int j = 1; j < msgSize; j++){
                message += " " + msgList.at(j);
            }
            broadcast->addEvent(QString("C ") + QString::number(this->playerNumber) + message);
        }
//    }
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
    myClock.restart();
    Last = sf::seconds(0);
    QPair<QString, float> moveString;
    bool queueEmpty = moveQueue->isEmpty();
    while(!moveQueue->isEmpty()){
        moveString = moveQueue->dequeue();
        movement.x = 0;
        movement.y = 0;
        switch(moveString.first.at(1).toLatin1()){
            case 'U' : movement.y -= movespeed;
                break;
            case 'D' : movement.y += movespeed;
                break;
            case 'R' : movement.x += movespeed;
                break;
            case 'L' : movement.x -= movespeed;
                break;
        }
        movement *= moveString.second;
        bool collided = false;
        for(const tmx::MapObject* now : map->QueryQuadTree(getBoundingBox())){
            if(now->GetName() == "Wall" || now->GetName() == "Edge"){
                QList<sf::Vector2f>::const_iterator itr = playerCollisionPoint.cbegin();
                QList<sf::Vector2f>::const_iterator end = playerCollisionPoint.cend();
                for(; itr != end; ++itr){
                    if(now->Contains( (*itr) )){
                        collided = true;
                        break;
                    }
                }
                if(collided){
                    break;
                }
            }
        }
        if(!collided){
            position += movement;
        }

        moveCounter++;
    }
    if(!queueEmpty){
        broadcast->addEvent("W " + QString::number(playerNumber) + QString(" ") + moveString.first.at(1) + QString(" ") + QString::number(position.x) + QString(" ") + QString::number(position.y) + "\n");
    }

}

sf::FloatRect ClientThread::getBoundingBox(){
    return sf::FloatRect(position.x, position.y, 32, 48);
}

void ClientThread::loadMap(){
    this->map = broadcast->getMap();
}

void ClientThread::startGame(){
    loadMap();
    //Send notification to player
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
