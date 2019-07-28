#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QObject>
#include <QtSql/QSqlDatabase>

typedef enum DB_ENTRY
{
    MESSAGE,
    STAT_BYTES_COUNT,
    STAT_RECONN_COUNT,
} DBEntryType;

static const QList<QPair<QString,QString>> DBStructure = {
    QPair<QString,QString>("Messages", "Message"),
    QPair<QString,QString>("Sessions", "BytesReceived"),
    QPair<QString,QString>("Sessions", "Reconnects")
};

class Persistence : public QObject
{
    Q_OBJECT
public:
    explicit Persistence(QObject *parent = nullptr);

    bool WriteEntry(DBEntryType type, QString message);

signals:

public slots:

private:
    bool initDatabaseConnection();
    QSqlDatabase m_database;
};

#endif // PERSISTENCE_H
