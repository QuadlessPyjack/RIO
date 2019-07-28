#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QSslKey>

// forward declaration
namespace RIOProtocol {
    struct Payload;
}

class SslClient : public QObject
{
    Q_OBJECT
public:
    explicit SslClient(QObject *parent = nullptr);
    ~SslClient();
    int NumberOfDisconnects;
    int NumberOfManualDisconnects;
    bool ConnectToServer();
    void DisconnectFromServer();
    void SendMessage(QString message);
signals:
    void disconnectedFromServer();
    void payloadReceived(RIOProtocol::Payload *payload);
    void latencyChanged(long latencyVal);

public slots:
    void onStatusInfoWriteRequested();

private slots:
    void onDataChannelDisconnected();
    void onStatusChannelDisconnected();
    void onSSlError(const QList<QSslError> &errors);
    void onDataChannelReceive();
    void onStatusChannelReceive();
    void onSocketStateChanged(QAbstractSocket::SocketState state);

private:
    bool connectSocket(QSslSocket *socket, uint16_t port);
    void setupSSLSockets();
    void loadSSLData();

    void checkLatency();
    uint8_t m_channelCount = 0;
    QSslKey m_key;
    QSslCertificate m_cert;
    QSslSocket *m_statsSocket;
    QSslSocket *m_connSocket;
};

#endif // SSLCLIENT_H
