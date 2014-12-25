#include "batchprocessor.h"

BatchProcessor::BatchProcessor(QLinkedList<ClientThread> *clientList, QObject *parent, int interval) :
    QThread(parent)
{
    processTimer.setInterval(interval);
    this->clientList = clientList;
}

void BatchProcessor::run(){
    OnStart();

    connect(&processTimer, SIGNAL(timeout()), this, SLOT(processQueue()));
    processTimer.start();
}

void BatchProcessor::addEvent(QString event){
    EventQueue.append(event);
}
