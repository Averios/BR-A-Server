#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QLinkedList>
#include <QQueue>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(qintptr ID, QLinkedList<ClientThread*>* clientList, QObject *parent = 0);
    virtual ~ClientThread();
    void run();
signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();
    void processQueue();

private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QLinkedList<ClientThread*>* clientList;
    QQueue<QString>* moveQueue;
    QTimer* processTimer;

};

#endif // CLIENTTHREAD_H
