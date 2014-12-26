#include <QCoreApplication>
#include "theserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TheServer server;
    server.startserver();
    return a.exec();
}
