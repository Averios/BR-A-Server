#ifndef BULLETCALCULATOR_H
#define BULLETCALCULATOR_H

#include <QThread>

class BulletCalculator : public QThread
{
    Q_OBJECT
public:
    explicit BulletCalculator(QObject *parent = 0);

signals:

public slots:

};

#endif // BULLETCALCULATOR_H
