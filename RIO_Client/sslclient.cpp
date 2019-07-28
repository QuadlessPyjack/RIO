#include "sslclient.h"
#include <QFile>
#include <QList>
#include <chrono>

#include <rio_protocol.h>

void SslClient::setupSSLSockets()
{
    m_statsSocket->setPrivateKey(m_key);
    m_statsSocket->setLocalCertificate(m_cert);
    m_connSocket->setPrivateKey(m_key);
    m_connSocket->setLocalCertificate(m_cert);
    m_statsSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    m_connSocket->setPeerVerifyMode(QSslSocket::VerifyNone);

    connect(m_statsSocket, &QSslSocket::readyRead, this, &SslClient::onStatusChannelReceive);
    connect(m_connSocket, &QSslSocket::readyRead, this, &SslClient::onDataChannelReceive);

    connect(m_connSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &SslClient::onSSlError);
    connect(m_statsSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &SslClient::onSSlError);
    connect(m_connSocket, &QSslSocket::disconnected,
            this, &SslClient::onDataChannelDisconnected);
    connect(m_statsSocket, &QSslSocket::disconnected,
            this, &SslClient::onStatusChannelDisconnected);
    connect(m_connSocket, &QSslSocket::stateChanged,
            this, &SslClient::onSocketStateChanged);
}

void SslClient::loadSSLData()
{
    QFile keyFile(":/certificates/server_local.key");
    keyFile.open(QIODevice::ReadOnly);
    m_key = QSslKey(keyFile.readAll(), QSsl::Rsa);
    keyFile.close();

    QFile certFile(":/certificates/server_local.pem");
    certFile.open(QIODevice::ReadOnly);
    m_cert = QSslCertificate(certFile.readAll());
    certFile.close();
}

SslClient::SslClient(QObject *parent) : QObject(parent)
  , NumberOfDisconnects(0)
  , NumberOfManualDisconnects(0)
  , m_statsSocket(new QSslSocket(this))
  , m_connSocket(new QSslSocket(this))
{
    loadSSLData();

    setupSSLSockets();
}

SslClient::~SslClient()
{
    delete m_statsSocket;
    delete m_connSocket;
    m_statsSocket = nullptr;
    m_connSocket = nullptr;
}

bool SslClient::ConnectToServer()
{
    return (
        connectSocket(m_connSocket,  RIOProtocol::COMM) &&
        connectSocket(m_statsSocket, RIOProtocol::STAT)
                );
}

void SslClient::DisconnectFromServer()
{
    NumberOfDisconnects++;
    NumberOfManualDisconnects++;
    m_connSocket->disconnectFromHost();
    m_statsSocket->disconnectFromHost();
}

void SslClient::SendMessage(QString message)
{
    if (!m_connSocket->isOpen() || !m_connSocket->isEncrypted())
    {
        return;
    }

    RIOProtocol::Payload p;
    p.version = 0;
    p.payloadType = RIOProtocol::MSG;
    std::string msg = message.toStdString();
    p.payloadCount = static_cast<uint32_t>(strlen(msg.c_str())) + 1;
    p.data = new uint8_t[p.payloadCount];
    memcpy(p.data, msg.c_str(), p.payloadCount);
    QByteArray ba = RIOProtocol::serializePayload(p);

    m_connSocket->write(ba);
}

void SslClient::onStatusInfoWriteRequested()
{
    checkLatency();
    // other metrics go here
}

bool SslClient::connectSocket(QSslSocket *socket, uint16_t port)
{
    socket->connectToHostEncrypted("127.0.0.1", port);
    if (socket->waitForEncrypted(port))
    {
        qDebug() << "auth = success";
        checkLatency();
        m_channelCount++;
        return true;
    }

    // auth = fail
    qDebug() << "Secured connection failed.";
    qDebug() << "Reason: " << socket->error();
    return false;
}

void SslClient::onStatusChannelDisconnected()
{
    m_channelCount--;
    if (m_channelCount <= 0)
    {
        NumberOfDisconnects++;
        emit disconnectedFromServer();
    }
    qDebug("Server disconnected");
    //exit(0);
}

void SslClient::onDataChannelDisconnected()
{
    m_channelCount--;
    if (m_channelCount <= 0)
    {
        NumberOfDisconnects++;
        emit disconnectedFromServer();
    }
    qDebug("Server disconnected");
    //exit(0);
}

void SslClient::onSSlError(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
    {
        qDebug() << error.errorString();
    }
}

void SslClient::onStatusChannelReceive()
{
    QByteArray ba = m_statsSocket->readAll();
    RIOProtocol::Payload p = RIOProtocol::deserializePayload(ba);

    emit payloadReceived(&p);
}

void SslClient::onDataChannelReceive()
{
    qDebug() << m_connSocket->readAll();
}

void SslClient::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Socket State is: " << state;
}

void SslClient::checkLatency()
{
    if (m_channelCount <= 0)
    {
        return;
    }

    RIOProtocol::Payload pingPayload;
    pingPayload.version = 0;
    pingPayload.payloadType = RIOProtocol::PING;
    pingPayload.payloadCount = 0;
    pingPayload.data = nullptr;

    QByteArray baPayload = RIOProtocol::serializePayload(pingPayload);

    using namespace std::chrono;
    auto timeStart = high_resolution_clock::now();
    m_statsSocket->write(baPayload);
    m_statsSocket->waitForReadyRead(5000);
    auto timeEnd = high_resolution_clock::now();
    QByteArray baResp = m_statsSocket->readAll();
    RIOProtocol::Payload resp = RIOProtocol::deserializePayload(baResp);

    long long duration = duration_cast<microseconds>(timeEnd - timeStart).count();
    if (duration >= 5000000)
    {
        return;
    }

    emit latencyChanged(duration);

    qDebug() << "Received following in t=" << duration;
    RIOProtocol::printPayload(resp);

}
