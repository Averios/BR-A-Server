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
    movespeed = 300;

    qDebug() << socketDescriptor << " Socket connected";

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
    qDebug() << stream;

        if(stream.at(0) == 'W'){
//            Elapsed = myClock.getElapsedTime() - Last;
//            Last = myClock.getElapsedTime();
            moveQueue->append(QPair<QString, float>(stream, Elapsed.asSeconds()));
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
//    qDebug() << data;
    this->socket->write(data.toUtf8());
//    this->socket->flush();
}

void ClientThread::disconnected(){
    qDebug() << "Disconnected";

    clientList->removeOne(this);
    socket->deleteLater();
    exit(0);
}

void ClientThread::processQueue(){
    //Do calculation and send to broadcaster
    Elapsed = myClock.restart();
    Last = sf::seconds(0);
    QPair<QString, float> moveString;
    bool queueEmpty = moveQueue->isEmpty();
    while(!moveQueue->isEmpty()){
        moveString = moveQueue->dequeue();
        movement.x = 0;
        movement.y = 0;
//        qDebug() << moveString.first;
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
        movement *= Elapsed.asSeconds();
        bool collided = false;
        for(const tmx::MapObject* now : map->QueryQuadTree(getBoundingBox())){
            if(now->GetName() == "Wall" || now->GetName() == "Edge"){
                if(now->GetAABB().intersects(sf::FloatRect(position.x + movement.x, position.y + movement.y + 32.f, 32.f, 16.f))){
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
        broadcast->addEvent(theEvent);
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
    QString message("P " + QString::number(playerNumber).toUtf8() + "\n");
    qDebug() << message;
    socket->write(message.toUtf8());
//    socket->flush();
//    broadcast->StartGame();
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
