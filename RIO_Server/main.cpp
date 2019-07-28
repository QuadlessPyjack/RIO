#include <QCoreApplication>
#include "connectionmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ConnectionManager conmgr;
    conmgr.Init();

    return a.exec();
}
