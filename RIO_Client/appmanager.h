#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QVector>
#include <QPointF>

//forward declarations
class SslClient;
class QQmlEngine;
class QJSEngine;
class QTimer;
namespace QtCharts {
    class QAbstractSeries;
}

namespace RIOProtocol {
    struct Payload;
}

class AppManager : public QObject
{
    Q_OBJECT
public:
    static AppManager* getInstance();
    static QObject* getQmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    ~AppManager();
    Q_INVOKABLE bool connectToServer();
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void applyTransmitInterval();
    Q_INVOKABLE void sendMessage();
    Q_INVOKABLE void updateLatencyPlotData(QtCharts::QAbstractSeries *series);
    Q_INVOKABLE void clearLatencyPlotData(QtCharts::QAbstractSeries *series);
    Q_INVOKABLE void setDisconnectSeriesPtr(QtCharts::QAbstractSeries *series);
    Q_PROPERTY(QString transmitInterval MEMBER m_transmitInterval NOTIFY transmitIntervalChanged)
    Q_PROPERTY(QString inputMessage MEMBER m_inputMessage NOTIFY inputMessageChanged)
    Q_PROPERTY(QString userDisconnects READ getUserDisconnects NOTIFY userDisconnectsChanged)
    Q_PROPERTY(QString serverDisconnects READ getServerDisconnects NOTIFY serverDisconnectsChanged)
    Q_PROPERTY(double latencyXMax READ getLatencyXMax NOTIFY latencyXMaxChanged)
    Q_PROPERTY(double latencyYMax READ getLatencyYMax NOTIFY latencyYMaxChanged)

    QString getUserDisconnects();
    QString getServerDisconnects();
    double getLatencyXMax();
    double getLatencyYMax();

signals:
    void transmitIntervalChanged();
    void inputMessageChanged();
    void userDisconnectsChanged();
    void serverDisconnectsChanged();
    void disconnectedFromServer();
    void connectedToServer();
    void latencyChanged();
    void latencyXMaxChanged();
    void latencyYMaxChanged();

private slots:
    void onDisconnectedFromServer();
    void onPayloadReceived(RIOProtocol::Payload *p);
    void onLatencyChanged(double newVal);

private:
    explicit AppManager(QObject *parent = nullptr);
    SslClient *m_sslClient;
    static AppManager* s_appManager;

    void addDisconnectSpike();

    QTimer *m_infoTransmitTimer;
    QString m_transmitInterval;
    QString m_inputMessage;
    int m_userDisconnects;
    int m_totalDisconnects;
    double m_latencyXMax;
    double m_latencyYMax;
    QVector<QPointF> m_latencyPoints;
    QtCharts::QAbstractSeries *m_disconnectSeries;
};

#endif // APPMANAGER_H
