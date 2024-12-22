#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "singleton.h"

#define XORKEY "2A#7F8D1C&6E"
#define TABLE_NAME "confinfo"

#define eBase DataBase::getInstance()

class DataBase : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(DataBase)
public:
    ~DataBase();
    bool setConf(const QString &group, const QString &key, const QVariant &value, bool encry = false);
    QString getConf(const QString &group, const QString &key, const QString &default_value = "", bool encry = false);

    bool setConfsByGroup(const QString &group, QMap<QString, QVariant> &map);
    QMap<QString, QString> getConfsByGroup(const QString &group);

private:
    DataBase();

    QSqlDatabase db;
    bool m_isOpen;

    bool openDb(const QString &dbName);
    void createTable(const QString &tableName);
    bool insertOrUpdate(const QString &group, const QString &key, const QString &value);

    QByteArray xorFun(const QByteArray &data, const QByteArray &key);
    QString encrypt(const QString &plainText, const QString &key);
    QString decrypt(const QString &cipherText, const QString &key);

};

#endif // DATABASE_H
