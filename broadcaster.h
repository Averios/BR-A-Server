#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "batchprocessor.h"
#include <QQueue>

class Broadcaster : public BatchProcessor
{
    Q_OBJECT
public:
    explicit Broadcaster(QLinkedList<ClientThread*>* clientList, QObject *parent = 0);
signals:

public slots:
    void processQueue();

private:
    void OnStart();

};

#endif // BROADCASTER_H
