#include "broadcaster.h"

Broadcaster::Broadcaster(QLinkedList<ClientThread *> *clientList, QObject *parent) :
    BatchProcessor(parent)
{
    this->clientList = clientList;
}

void Broadcaster::OnStart(){

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

void Broadcaster::addEvent(QString event){
    EventQueue << event;
}
