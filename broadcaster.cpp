#include "broadcaster.h"

Broadcaster::Broadcaster(QLinkedList<ClientThread *> *clientList, QObject *parent) :
    BatchProcessor(parent)
{
    this->clientList = clientList;
    this->map = new tmx::MapLoader("Resources");
}

void Broadcaster::OnStart(){
    this->map->Load("exampleMap.tmx");
    this->map->UpdateQuadTree(sf::FloatRect(0.f, 0.f, map->GetMapSize().x, map->GetMapSize().y));
}

void Broadcaster::processQueue(){
    QString eventNow;

    while(!EventQueue.isEmpty()){
        eventNow = EventQueue.dequeue();
        QLinkedList<ClientThread*>::const_iterator itr = clientList->cbegin();
        QLinkedList<ClientThread*>::const_iterator end = clientList->cend();
        for(; itr != end; ++itr){
            (*itr)->sendData(eventNow);
        }
    }
}

tmx::MapLoader* Broadcaster::getMap(){
    return this->map;
}

void Broadcaster::addEvent(QString event){
    EventQueue << event;
}
