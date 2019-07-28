#include "connectionmanager.h"
#include "persistence.h"
#include "sslserver.h"

#include <QTimer>
#include <QMap>

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent)
  , m_timeoutMs(40000)
  , m_timeoutTimer(new QTimer(this))
  , m_persistence(new Persistence(this))
  , m_statsServer(new SslServer(this))
  , m_connServer(new SslServer(this))
{
    m_statsServer->id = RIOProtocol::STAT;
    m_connServer->id = RIOProtocol::COMM;
}

void ConnectionManager::startListening()
{
    m_statsServer->listen(QHostAddress("127.0.0.1"), RIOProtocol::STAT);
    qDebug() << "Attempting to listen on data port";
    m_connServer->listen(QHostAddress("127.0.0.1"), RIOProtocol::COMM);
}

void ConnectionManager::Init()
{
    connect(m_statsServer, &SslServer::payloadReceived,
            this, &ConnectionManager::handleIncomingData);
    connect(m_connServer, &SslServer::payloadReceived,
            this, &ConnectionManager::handleIncomingData);

    connect(m_timeoutTimer, &QTimer::timeout,
            this, &ConnectionManager::disconnectClient);

    connect(m_connServer, &SslServer::startConnectionTimeoutTimer,
            this, &ConnectionManager::onTimerStart);
    connect(m_connServer, &SslServer::resetConnectionTimeoutTimer,
            this, &ConnectionManager::onTimerReset);

    connect(m_statsServer, &SslServer::clientConnected,
            this, &ConnectionManager::onClientConnected);
    connect(m_connServer, &SslServer::clientConnected,
            this, &ConnectionManager::onClientConnected);


    qDebug() << "Attempting to listen on stats port";
    startListening();

    m_timeoutTimer->setSingleShot(false);
    m_timeoutTimer->setInterval(m_timeoutMs);
}

void ConnectionManager::deinit()
{
    disconnect(m_statsServer, &SslServer::payloadReceived,
            this, &ConnectionManager::handleIncomingData);
    disconnect(m_connServer, &SslServer::payloadReceived,
            this, &ConnectionManager::handleIncomingData);

    disconnect(m_timeoutTimer, &QTimer::timeout,
            this, &ConnectionManager::disconnectClient);

    disconnect(m_connServer, &SslServer::startConnectionTimeoutTimer,
            this, &ConnectionManager::onTimerStart);
    disconnect(m_connServer, &SslServer::resetConnectionTimeoutTimer,
            this, &ConnectionManager::onTimerReset);

    disconnect(m_statsServer, &SslServer::clientConnected,
            this, &ConnectionManager::onClientConnected);
    disconnect(m_connServer, &SslServer::clientConnected,
            this, &ConnectionManager::onClientConnected);
}

ConnectionManager::~ConnectionManager()
{
    delete m_statsServer;
    delete m_connServer;

    m_statsServer = nullptr;
    m_connServer = nullptr;
}

void ConnectionManager::onTimerReset()
{
    m_timeoutTimer->stop();
    m_timeoutTimer->start(m_timeoutMs);
}

void ConnectionManager::onTimerStart()
{
    m_timeoutTimer->start(m_timeoutMs);
}

void ConnectionManager::onClientConnected(QTcpSocket *clientSocket)
{
    QString clientAddress = clientSocket->peerAddress().toString();
    quint16 localPort = clientSocket->localPort();

    auto it = m_connectedClients.find(clientAddress);

    qDebug() << clientSocket->localPort();
    // complete the pair
    if (it != m_connectedClients.end())
    {
        auto clientPair = it.value();
        if (localPort == RIOProtocol::COMM)
        {
            clientPair.first = clientSocket;
            m_connectedClients.insert(clientAddress, clientPair);
        }
        if (localPort == RIOProtocol::STAT)
        {
            clientPair.second = clientSocket;
            m_connectedClients.insert(clientAddress, clientPair);
        }
        return;
    }

    // pair not found, create a new one
    QPair<QTcpSocket*,QTcpSocket*> client;
    if (localPort == RIOProtocol::COMM)
    {
        client.first = clientSocket;
    }
    if (localPort == RIOProtocol::STAT)
    {
        //qDebug() << "Second socket: " << clientSocket;
        client.second = clientSocket;
    }

    m_connectedClients.insert(clientAddress, client);
    m_persistence;
}

void ConnectionManager::handleIncomingData(RIOProtocol::Payload p, QTcpSocket *clientSocket)
{
    switch (p.payloadType)
    {
        case RIOProtocol::MSG:
        {
            QString message = QString(reinterpret_cast<char*>(p.data));
            qDebug() << "Received string: " << message;
            m_persistence->WriteEntry(DBEntryType::MESSAGE, message);
            break;
        }
        case RIOProtocol::PING:
        {
            qDebug() << "Received PING packet";
            RIOProtocol::Payload p = RIOProtocol::generatePingPayload();
            QByteArray ba = RIOProtocol::serializePayload(p);

            clientSocket->write(ba);
            break;
        }
        case RIOProtocol::INFO:
        {
            qDebug() << "Received client session info";
            break;
        }
        default:
            break;
    }
}

void ConnectionManager::disconnectClient()
{
    // normally we should instantiate a separate timer for each entry in the m_connectedClients map
    // but we're running out of time

    if (m_connectedClients.count() == 0)
    {
        return;
    }

    auto client = m_connectedClients.first();
    QString peerAddress = client.first->peerAddress().toString();

    client.first->disconnectFromHost();
    client.second->disconnectFromHost();

    client.first->disconnectFromHost();
    client.second->disconnectFromHost();

    m_connectedClients.remove(peerAddress);
}
