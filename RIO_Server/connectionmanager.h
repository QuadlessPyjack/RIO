#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "persistence.h"

#include <QObject>
#include <QSslSocket>

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = nullptr);

signals:

public slots:

private:
    QSslSocket *m_statsSocket;
    QSslSocket *m_connSocket;

    Persistence m_persistence;
};

#endif // CONNECTIONMANAGER_H
