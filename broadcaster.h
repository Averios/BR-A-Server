#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <QThread>
#include <QQueue>

class Broadcaster : public QThread
{
    Q_OBJECT
public:
    explicit Broadcaster(QObject *parent = 0);
    void addMessage(QString message);
signals:

public slots:

private:
    QQueue<QString> messageQueue;

};

#endif // BROADCASTER_H
