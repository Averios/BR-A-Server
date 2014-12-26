#ifndef BATCHPROCESSOR_H
#define BATCHPROCESSOR_H

#include <QThread>
#include <QTimer>
#include <QQueue>
#include <QLinkedList>
#include <tmx/MapLoader.h>


class BatchProcessor : public QThread
{
    Q_OBJECT
public:
    explicit BatchProcessor(QObject *parent = 0, int interval = 10);
    void run();
    virtual void addEvent(QString event) = 0;
signals:

public slots:
    virtual void processQueue() = 0;
private:
    virtual void OnStart() = 0;
    QTimer processTimer;
protected:
    QQueue<QString> EventQueue;
    tmx::MapLoader* map;
};

#endif // BATCHPROCESSOR_H
