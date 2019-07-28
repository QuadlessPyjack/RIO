#include "persistence.h"

#include <QtSql/QSqlQuery>
#include <QVariant>

Persistence::Persistence(QObject *parent) : QObject(parent)
{

}

bool Persistence::WriteEntry(DBEntryType type, QString message)
{
    if (!m_database.isOpen())
    {
        return false;
    }

    m_database.transaction();
    QSqlQuery query = QSqlQuery(QString("INSERT INTO %1 (%2)"
                                        "VALUES (:message)").arg(DBStructure[type].first).arg(DBStructure[type].second));

    query.bindValue(":message", message);
    bool status = query.exec();

    if (!status)
    {
        m_database.rollback();
        return false;
    }

    m_database.commit();
    return true;
}

bool Persistence::initDatabaseConnection()
{
    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName("localhost");
    m_database.setDatabaseName("testdb");
    m_database.setUserName("sa");
    m_database.setPassword("test");

    bool status = m_database.open();

    return status;
}
