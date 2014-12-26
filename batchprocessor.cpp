#include "batchprocessor.h"

BatchProcessor::BatchProcessor(QObject *parent, int interval) :
    QThread(parent)
{
    processTimer.setInterval(interval);
}

void BatchProcessor::run(){
    OnStart();

    connect(&processTimer, SIGNAL(timeout()), this, SLOT(processQueue()));
    processTimer.start();
}
