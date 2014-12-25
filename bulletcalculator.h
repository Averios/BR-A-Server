#ifndef BULLETCALCULATOR_H
#define BULLETCALCULATOR_H

#include "batchprocessor.h"

class BulletCalculator : public BatchProcessor
{
    Q_OBJECT
public:
    explicit BulletCalculator(QLinkedList<ClientThread*> clientList, QObject *parent = 0);

signals:

public slots:
    void processQueue();

private:
    void OnStart();
};

#endif // BULLETCALCULATOR_H
