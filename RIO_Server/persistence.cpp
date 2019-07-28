#include "persistence.h"

#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

Persistence::Persistence(QObject *parent) : QObject(parent)
{
    bool isOk = initDatabaseConnection();
    if (isOk)
    {
        QString dateTimeString = QDateTime::currentDateTime().toString();
        WriteEntry(DBEntryType::MESSAGE, "'DB Init OK [" + dateTimeString + "]'");
    }
}

bool Persistence::WriteEntry(DBEntryType type, QString message)
{
    if (!m_database.isOpen())
    {
        return false;
    }

    m_database.transaction();
    QString queryString = QString("INSERT INTO %1 (%2)"
                                  "VALUES ('%3');").arg(DBStructure[type].first).arg(DBStructure[type].second)
                                                .arg(message);
    QSqlQuery query = QSqlQuery(queryString);
    bool status = query.exec();

    if (!status)
    {
        m_database.rollback();
        qDebug() << "Executing query: ";
        qDebug() << queryString;
        qDebug() << "FAILED. Reason: ";
        qDebug() << query.lastError();
        return false;
    }

    m_database.commit();
    return true;
}

bool Persistence::initDatabaseConnection()
{
    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName("localhost");
    m_database.setDatabaseName("RIODB");
    m_database.setUserName("root");
    m_database.setPassword("1234#");

    bool status = m_database.open();

    return status;
}
