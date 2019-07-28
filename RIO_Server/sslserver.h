#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>

#include <rio_protocol.h>

class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    SslServer(QObject *parent = nullptr);
    int id;

signals:
    void payloadReceived(RIOProtocol::Payload p, QTcpSocket *sendingSocket);
    void clientConnected(QTcpSocket *clientSocket);
    void clientDisconnected();
    void startConnectionTimeoutTimer();
    void resetConnectionTimeoutTimer();

private:

    void incomingConnection(qintptr socketDescriptor);

    void onDisconnected();
    void onSSlError(const QList<QSslError> &errors);
    void onReceive();

    void link();

    QSslKey m_key;
    QSslCertificate m_cert;
};

#endif // SSLSERVER_H
