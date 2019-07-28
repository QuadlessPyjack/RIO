#include "sslserver.h"

#include <QFile>

SslServer::SslServer(QObject *parent)
    : QTcpServer (parent)
{
    QFile keyFile(":/certificates/client_local.key");
    keyFile.open(QIODevice::ReadOnly);
    m_key = QSslKey(keyFile.readAll(), QSsl::Rsa);
    keyFile.close();

    QFile certFile(":/certificates/client_local.pem");
    certFile.open(QIODevice::ReadOnly);
    m_cert = QSslCertificate(certFile.readAll());
    certFile.close();

    connect(this, &SslServer::newConnection, this, &SslServer::link);
}

void SslServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);

    connect(sslSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &SslServer::onSSlError);

    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPrivateKey(m_key);
    sslSocket->setLocalCertificate(m_cert);
    qDebug() << "Attempting to add CACertificates to socket... ";
    bool isCaCertAddSuccessful = sslSocket->addCaCertificates(":/certificates/server_ca.pem");
    isCaCertAddSuccessful ? qDebug() << "SUCCESS" : qDebug() << "FAIL";
    sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);
    sslSocket->startServerEncryption();

    addPendingConnection(sslSocket);
    emit startConnectionTimeoutTimer();

    emit clientConnected(qobject_cast<QTcpSocket*>(sslSocket));
}

void SslServer::onDisconnected()
{
    qDebug("Client Disconnected");
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    this->disconnect(clientSocket);
    clientSocket->disconnect(this);
    clientSocket->deleteLater();
}

void SslServer::onSSlError(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
    {
        qDebug() << error.errorString();
    }
}

void SslServer::onReceive()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    QByteArray ba = clientSocket->readAll();
    RIOProtocol::Payload rec = RIOProtocol::deserializePayload(ba);

    emit payloadReceived(rec, clientSocket);
    emit resetConnectionTimeoutTimer();
}

void SslServer::link()
{
    QTcpSocket *clientSocket = nullptr;

    clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &SslServer::onReceive);
    connect(clientSocket, &QTcpSocket::disconnected, this, &SslServer::onDisconnected);
}
