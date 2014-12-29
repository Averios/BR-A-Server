#include "broadcaster.h"

Broadcaster::Broadcaster(QList<ClientThread *> *clientList, QObject *parent) :
    BatchProcessor(parent),
    rng(rd())
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
        QList<ClientThread*>::const_iterator itr = clientList->cbegin();
        QList<ClientThread*>::const_iterator end = clientList->cend();
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

void Broadcaster::initiatePosition(){
    if(SpawnPoint.empty()){
        for(const tmx::MapLayer layers : map->GetLayers()){
            if(layers.name == "Spawn"){
                for(const tmx::MapObject point : layers.objects){
                    SpawnPoint.push_back(point.GetPosition());
                }
            }
        }
    }

    std::shuffle(SpawnPoint.begin(), SpawnPoint.end(), rng);
    int playerSize = clientList->size();
    for(int i = 0; i < playerSize; i++){
        clientList->at(i)->setInitialPosition(SpawnPoint.at(i));
        EventQueue.append( "WD " + QString::number(clientList->at(i)->getNumber()) + QString(" ") + QString::number(SpawnPoint.at(i).x) + QString(" ") + QString::number(SpawnPoint.at(i).y) + "\n");
    }
}

void Broadcaster::StartGame(){
    initiatePosition();
    EventQueue.append("GS\n");
}

void Broadcaster::respawn(int player){
    std::uniform_int_distribution<> rand(0, SpawnPoint.size() - 1);
    sf::Vector2f point = SpawnPoint.at(rand(rng));
    clientList->at(player)->setInitialPosition(point);
    EventQueue.append( "WD " + QString::number(player) + QString(" ") + QString::number(point.x) + QString(" ") + QString::number(point.y) + "\n");
}
