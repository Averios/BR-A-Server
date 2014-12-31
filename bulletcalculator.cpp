#include "bulletcalculator.h"

BulletCalculator::BulletCalculator(QList<ClientThread *>* clientList, QObject *parent) :
    BatchProcessor(parent)
{
    this->clientList = clientList;
    bulletSpeed = 1000.f;
}

void BulletCalculator::addBroadcaster(Broadcaster *broadcast){
    this->broadcast = broadcast;
}

void BulletCalculator::addEvent(QString event){
    EventQueue << event;
    broadcast->addEvent(event);
}

void BulletCalculator::OnStart(){
    //Do something maybe?
    loadMap();
}

void BulletCalculator::processQueue(){
    Elapsed = myClock.restart();
    //Do calculation
    QString eventString;
    while(!EventQueue.isEmpty()){
        eventString = EventQueue.dequeue();
        QStringList stringList = eventString.split(" ");
        if(stringList.size() < 4)
            continue;
        sf::Vector2f pos(stringList[1].toDouble(), stringList[2].toDouble());
        Bullet newBullet(pos, stringList[3].toDouble());
        newBullet.setSender(stringList[4].toInt());
        newBullet.setSpeed(500);
        BulletList.append(newBullet);
    }

    for(Bullet& now : BulletList){
        now.update(Elapsed.asSeconds());
    }

    int counter = 0;
    for(Bullet& now : BulletList){
        bool collided = false;
        for(const tmx::MapObject* objects : map->QueryQuadTree(now.getBoundingBox())){
            if(objects->GetName() == "Wall" && now.getBoundingBox().intersects(objects->GetAABB())){
                collided = true;
                break;
            }
        }
        if(collided){
            BulletList.removeAt(counter);
        }
        else{
            bool collide2 = false;
            for(ClientThread* player : (*clientList)){
                if(player->getNumber() == now.getSender()) continue;
                if(now.getBoundingBox().intersects(player->getBoundingBox())){
                    broadcast->addEvent("K " + QString::number(now.getSender()) + QString(" ") + QString::number(player->getNumber()) + "\n");
                    //Send the respawn point to player
                    broadcast->respawn(player->getNumber());
                    collide2 = true;
                    break;
                }
            }
            if(collide2){
                BulletList.removeAt(counter);
            }
        }
        counter++;
    }
//    for(const Bullet& now : removal){
//        BulletList.re
//    }
//    removal.clear();
}

void BulletCalculator::loadMap(){
    this->map = broadcast->getMap();
}
