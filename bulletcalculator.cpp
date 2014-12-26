#include "bulletcalculator.h"

BulletCalculator::BulletCalculator(QLinkedList<ClientThread *>* clientList, QObject *parent) :
    BatchProcessor(parent)
{
    this->clientList = clientList;
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
}

void BulletCalculator::processQueue(){
    Elapsed = myClock.restart();
    //Do calculation
}
