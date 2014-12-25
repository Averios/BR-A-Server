#include "bulletcalculator.h"

BulletCalculator::BulletCalculator(QLinkedList<ClientThread *> clientList, QObject *parent) :
    BatchProcessor(clientList, parent)
{
}
