#ifndef BATCHPROCESSOR_H
#define BATCHPROCESSOR_H

#include <QThread>
#include <QTimer>
#include <QQueue>
#include <QLinkedList>
#include "clientthread.h"

class BatchProcessor : public QThread
{
    Q_OBJECT
public:
    explicit BatchProcessor(QLinkedList<ClientThread>* clientList, QObject *parent = 0, int interval = 10);
    void run();
    void addEvent(QString event);
signals:

public slots:
    virtual void processQueue() = 0;
private:
    virtual void OnStart() = 0;

    QTimer processTimer;
    QQueue<QString> EventQueue;
    QLinkedList<ClientThread*>* clientList;
};

#endif // BATCHPROCESSOR_H
