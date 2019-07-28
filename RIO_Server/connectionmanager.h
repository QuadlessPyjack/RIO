#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QMap>
#include <QPair>

// forward declarations
class Persistence;
class SslServer;
class QTcpSocket;
class QTimer;

namespace RIOProtocol {
    struct Payload;
}

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = nullptr);

    void Init();

    ~ConnectionManager();
signals:

public slots:
    void onTimerReset();
    void onTimerStart();
    void onClientConnected(QTcpSocket *clientSocket);

private:
    void handleIncomingData(RIOProtocol::Payload p, QTcpSocket *clientSocket);
    void disconnectClient();

    QMap<QString,QPair<QTcpSocket*,QTcpSocket*>> m_connectedClients;

    int m_timeoutMs;
    QTimer *m_timeoutTimer;
    Persistence *m_persistence;
    SslServer *m_statsServer;
    SslServer *m_connServer;
    void startListening();
    void deinit();
};

#endif // CONNECTIONMANAGER_H
