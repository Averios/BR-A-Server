#include "broadcaster.h"

Broadcaster::Broadcaster(QList<ClientThread *> *clientList, QObject *parent) :
    BatchProcessor(parent),
    rng(rd())
{
    this->clientList = clientList;
    this->map = new tmx::MapLoader("Resources");
    started = false;
}

void Broadcaster::OnStart(){
    this->map->Load("exampleMap.tmx");
    this->map->UpdateQuadTree(sf::FloatRect(0.f, 0.f, map->GetMapSize().x, map->GetMapSize().y));
}

void Broadcaster::processQueue(){
    QString eventNow;
//    qDebug() << EventQueue.size();
//    if(!started && clientList->size() > 0){
//        this->StartGame();
//        started = true;
//    }
    while(!EventQueue.isEmpty()){
        eventNow = EventQueue.dequeue();
//        std::cout << eventNow.toStdString() << std::endl;
        if(eventNow == "GS\n"){
            this->StartGame();
        }
        int size = clientList->size();
        for(int i = 0; i < size; i++){
            clientList->at(i)->sendData(eventNow);
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
    QString newZ("N ");
    for(ClientThread* playa : *clientList){
        newZ += QString::number(playa->getNumber()) + QString(" ");
        playa->resetCounter();
    }
    newZ += "\n";
    EventQueue.append(newZ);
    std::shuffle(SpawnPoint.begin(), SpawnPoint.end(), rng);
    int playerSize = clientList->size();
//    qDebug() << playerSize;
    for(int i = 0; i < playerSize; i++){
        clientList->at(i)->setInitialPosition(SpawnPoint.at(i));
        EventQueue.append( "WD " + QString::number(clientList->at(i)->getNumber()) + QString(" ") + QString::number(SpawnPoint.at(i).x) + QString(" ") + QString::number(SpawnPoint.at(i).y) + QString(" ") + QString::number(0) +"\n");
    }
}

void Broadcaster::StartGame(){
    qDebug() << "Game Started";
    initiatePosition();
//    EventQueue.append("GS\n");
}

void Broadcaster::respawn(int player){
    std::uniform_int_distribution<> rand(0, SpawnPoint.size() - 1);
    sf::Vector2f point = SpawnPoint.at(rand(rng));
    clientMap->value(player)->setInitialPosition(point);
    EventQueue.append( "WD " + QString::number(player) + QString(" ") + QString::number(point.x) + QString(" ") + QString::number(point.y) + QString(" ") + QString::number(0) + "\n");
}

void Broadcaster::setMap(QMap<int, ClientThread *> *clientMap){
    this->clientMap = clientMap;
}
