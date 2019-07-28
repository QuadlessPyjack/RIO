#include "appmanager.h"
#include "sslclient.h"

#include <QTimer>
#include <QVector>
#include <QtCharts/qxyseries.h>
#include <QtCharts/QAbstractSeries>
#include <rio_protocol.h>

AppManager* AppManager::s_appManager = nullptr;

AppManager::AppManager(QObject *parent) : QObject(parent)
,m_sslClient(new SslClient(this))
,m_infoTransmitTimer(new QTimer(this))
,m_transmitInterval("5")
,m_inputMessage("")
,m_userDisconnects(0)
,m_totalDisconnects(0)
,m_latencyXMax(0)
,m_latencyYMax(0)
,m_latencyPoints({QPointF(0,0.0)})
,m_disconnectSeries(nullptr)
{
    qDebug() << "App manager constructor";

    connect(this, &AppManager::transmitIntervalChanged,
            this, &AppManager::applyTransmitInterval);

    connect(this, &AppManager::inputMessageChanged,
            this, &AppManager::sendMessage);

    m_infoTransmitTimer->setInterval(5000);
    m_infoTransmitTimer->setSingleShot(false);
}

void AppManager::addDisconnectSpike()
{
    if (m_disconnectSeries == nullptr)
    {
        return;
    }

    QPointF disconnectPoint;
    QPointF zeroPointPre = QPointF(m_latencyXMax - 0.2,0);
    QPointF zeroPointPost = QPointF(m_latencyXMax + 0.2,0);
    disconnectPoint.setX(m_latencyXMax);
    disconnectPoint.setY(m_latencyYMax);
    QtCharts::QXYSeries *points = static_cast<QtCharts::QXYSeries*>(m_disconnectSeries);
    points->append(zeroPointPre);
    points->append(disconnectPoint);
    points->append(zeroPointPost);
}

AppManager* AppManager::getInstance()
{
    if (s_appManager == nullptr)
    {
        s_appManager = new AppManager();
        return s_appManager;
    }

    return s_appManager;
}

QObject *AppManager::getQmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    QObject* qmlSingleton = getInstance();

    return qmlSingleton;
}

AppManager::~AppManager()
{
    qDebug() << "App manager Destructor";
    delete m_sslClient;
    m_sslClient = nullptr;
}

bool AppManager::connectToServer()
{
    qDebug() << "App manager connect to server";
    bool isOk = m_sslClient->ConnectToServer();

    if (!isOk)
    {
        emit disconnectedFromServer();
        return false;
    }

    connect(m_infoTransmitTimer, &QTimer::timeout,
            m_sslClient, &SslClient::onStatusInfoWriteRequested);
    connect(m_sslClient, &SslClient::disconnectedFromServer,
            this, &AppManager::onDisconnectedFromServer);
    connect(m_sslClient, &SslClient::payloadReceived,
            this, &AppManager::onPayloadReceived);
    connect(m_sslClient, &SslClient::latencyChanged,
            this, &AppManager::onLatencyChanged);

    m_infoTransmitTimer->start();
    emit connectedToServer();

    return true;
}

void AppManager::onDisconnectedFromServer()
{
    emit disconnectedFromServer();
    m_totalDisconnects = m_sslClient->NumberOfDisconnects;
    emit userDisconnectsChanged();
    emit serverDisconnectsChanged();

    addDisconnectSpike();
}

void AppManager::onPayloadReceived(RIOProtocol::Payload *p)
{
    switch (p->payloadType)
    {
        case RIOProtocol::PING:
        {

        }
    }
}

void AppManager::onLatencyChanged(double newVal)
{
    // first = X = time
    // second = Y = latency

    QPointF latencyPoint;
    double xValue = 0;

    if (!m_latencyPoints.isEmpty())
    {
        auto last = m_latencyPoints.last();
        xValue = last.x();

        // not optimal solution - time keeps increasing linearly with X
        for(auto point : m_latencyPoints)
        {
            if (m_latencyYMax < point.y())
            {
                m_latencyYMax = point.y();
            }
        }
        m_latencyYMax += 50;
        emit latencyYMaxChanged();
    } else {
        m_latencyYMax = newVal + 50;
        emit latencyYMaxChanged();
    }

    xValue = m_transmitInterval.toInt();
    latencyPoint.setX(m_latencyXMax);
    latencyPoint.setY(newVal);

    qDebug() << "xValue = " << xValue;
    m_latencyXMax += xValue;
    qDebug() << "m_latencyXMax = " << m_latencyXMax;

    m_latencyPoints.push_front(latencyPoint);
    emit latencyChanged();
    emit latencyXMaxChanged();
}

void AppManager::disconnectFromServer()
{
    m_infoTransmitTimer->stop();
    m_sslClient->DisconnectFromServer();
    m_userDisconnects = m_sslClient->NumberOfManualDisconnects;
    emit userDisconnectsChanged();
}

void AppManager::applyTransmitInterval()
{
    int timerValue = m_transmitInterval.toInt() * 1000;
    m_infoTransmitTimer->stop();
    m_infoTransmitTimer->setInterval(timerValue);
    m_infoTransmitTimer->start();
}

void AppManager::sendMessage()
{
    m_sslClient->SendMessage(m_inputMessage);
    qDebug() << "Sending message: " << m_inputMessage << " to SSLClient for Tx";
}

void AppManager::updateLatencyPlotData(QtCharts::QAbstractSeries *series)
{
    if (series == nullptr)
    {
        return;
    }

    QtCharts::QXYSeries *points = static_cast<QtCharts::QXYSeries*>(series);
    points->replace(m_latencyPoints);
}

void AppManager::clearLatencyPlotData(QtCharts::QAbstractSeries *series)
{
    if (series == nullptr)
    {
        return;
    }

    QtCharts::QXYSeries *points = static_cast<QtCharts::QXYSeries*>(series);
    points->clear();
}

void AppManager::setDisconnectSeriesPtr(QtCharts::QAbstractSeries *series)
{
    m_disconnectSeries = series;
}

QString AppManager::getUserDisconnects()
{
    QString userDisconnects = QString::number(m_userDisconnects);
    return userDisconnects;
}

QString AppManager::getServerDisconnects()
{
    QString serverDisconnects = QString::number(m_totalDisconnects - m_userDisconnects);
    return serverDisconnects;
}

double AppManager::getLatencyXMax()
{
    return m_latencyXMax;
}

double AppManager::getLatencyYMax()
{
    return m_latencyYMax;
}
