#include "database.h"
#include <QDir>
Q_SINGLETON_CREATE_CPP(DataBase)
DataBase::DataBase() : QObject()
{
    QString confDirPath = "./conf";
    QDir dir;
    if (!dir.exists(confDirPath)) {
        bool created = dir.mkpath(confDirPath);
        if (created) {
        }
    }
    m_isOpen = openDb("./conf/conf.db");
    if (m_isOpen) {
        createTable(TABLE_NAME);
    }
}

DataBase::~DataBase()
{

}

QByteArray DataBase::xorFun(const QByteArray &data, const QByteArray &key) {
    QByteArray result = data;
    int keyLen = key.size();

    for (int i = 0; i < result.size(); ++i) {
        result[i] = result[i] ^ key[i % keyLen];
    }
    return result;
}

QString DataBase::encrypt(const QString &plainText, const QString &key) {
    QByteArray base64Data = plainText.toUtf8().toBase64();
    QByteArray encryptedData = xorFun(base64Data, key.toUtf8());
    return encryptedData.toBase64();
}

QString DataBase::decrypt(const QString &cipherText, const QString &key) {
    QByteArray encryptedData = QByteArray::fromBase64(cipherText.toUtf8());
    QByteArray base64Data = xorFun(encryptedData, key.toUtf8());
    QByteArray decryptedData = QByteArray::fromBase64(base64Data);
    return QString::fromUtf8(decryptedData);
}

bool DataBase::openDb(const QString &dbName)
{
    if (QSqlDatabase::contains("default_connection")) {
        db = QSqlDatabase::database("default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName(dbName);
    if (!db.open()) {
        QString msg = db.lastError().text();
        qDebug() << __FUNCTION__ << "Error: " << msg;
        return false;
    } else {
        return true;
    }
}

void DataBase::createTable(const QString &tableName)
{
    QSqlQuery query(db);
    if (!query.exec(QString("CREATE TABLE IF NOT EXISTS %1 ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "\"group\" TEXT NOT NULL, "
                            "\"key\" TEXT NOT NULL, "
                            "value TEXT, "
                            "UNIQUE(\"group\", \"key\") "
                            ")").arg(tableName))) {
        qDebug() << Q_FUNC_INFO << "Error: " << query.lastError().text();
    }
}

bool DataBase::insertOrUpdate(const QString &group, const QString &key, const QString &value)
{
    if (!m_isOpen)
        return false;
    QSqlQuery query(db);
    QString sql = QString("INSERT INTO %1 (\"group\", \"key\", value) "
                          "VALUES (:group, :key, :value) "
                          "ON CONFLICT (\"group\", \"key\") DO UPDATE SET value = excluded.value").arg(TABLE_NAME);

    if (!query.prepare(sql)) {
        qDebug() << "Prepare failed:" << query.lastError();
        return false;
    }

    query.bindValue(":group", group);
    query.bindValue(":key", key);
    query.bindValue(":value", value);

    if (!query.exec()) {
        qDebug() << "Execution failed:" << query.lastError();
        return false;
    }
    return true;
}

bool DataBase::setConf(const QString &group, const QString &key, const QVariant &value, bool encry)
{
    QString tmp_value = value.toString();
    if (value.typeId() == QMetaType::Bool) {
        tmp_value = value.toBool() ? "2" : "0";
    }
    if (encry) {
        tmp_value = encrypt(tmp_value, XORKEY);
    }
    return insertOrUpdate(group, key, tmp_value);
}

bool DataBase::setConfsByGroup(const QString &groupName, QMap<QString, QVariant> &map)
{
    if (!m_isOpen)
        return false;

    QSqlQuery query(db);
    bool success = true;

    // 确保数据库已打开并开始事务
    if (!db.isOpen() || !db.transaction()) {
        qDebug() << __FUNCTION__ << "Error: Failed to start transaction or database not open."
                 << db.lastError().text();
        return false;
    }

    // 准备SQL语句，确保表名是安全的
    const QString sql = QString("INSERT INTO \"%1\" (\"group\", \"key\", \"value\") "
                                "VALUES (:group, :key, :value) "
                                "ON CONFLICT (\"group\", \"key\") DO UPDATE SET \"value\" = excluded.\"value\"")
                            .arg(TABLE_NAME);

    query.prepare(sql);
    query.bindValue(":group", groupName); // 绑定一次group参数

    for (auto it = map.cbegin(); it != map.cend(); ++it) {
        QVariant value = it.value();
        if (value.typeId() == QMetaType::Bool) {
            value = value.toBool(); // 假设数据库支持布尔类型
        }
        query.bindValue(":key", it.key());
        query.bindValue(":value", value);
        if (!query.exec()) {
            qDebug() << __FUNCTION__ << "Error: Failed to execute query for key:" << it.key()
            << "Error:" << query.lastError().text();
            success = false;
            break;
        }
    }

    if (success) {
        if (!db.commit()) {
            qDebug() << __FUNCTION__ << "Error: Failed to commit transaction" << db.lastError().text();
            return false;
        }
    } else {
        db.rollback();
        return false; // 直接返回false
    }

    return success;
}

QString DataBase::getConf(const QString &group, const QString &key, const QString &default_value, bool encry)
{
    if (!m_isOpen)
        return "";

    QSqlQuery query(db);

    if (!query.prepare(QString("SELECT value FROM %1 WHERE \"group\" = :group_value AND \"key\" = :key_value").arg(TABLE_NAME))) {
        qDebug() << Q_FUNC_INFO << "Error preparing query:" << query.lastError().text();
        return default_value;
    }

    query.bindValue(":group_value", group);
    query.bindValue(":key_value", key);

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << "Error executing query:" << query.lastError().text();
        return default_value;
    }

    if (query.next()) {
        QString value = query.value(0).toString();

        if (encry) {
            value = decrypt(value, XORKEY);
        }

        return value.isEmpty() ? default_value : value;
    }

    return default_value;
}

QMap<QString, QString> DataBase::getConfsByGroup(const QString &group)
{
    QMap<QString, QString> result;
    if (!m_isOpen)
        return result;

    QSqlQuery query(db);

    if (!query.prepare(QString("SELECT \"key\", value FROM %1 WHERE \"group\" = :group_value").arg(TABLE_NAME))) {
        qDebug() << Q_FUNC_INFO << "Error preparing query:" << query.lastError().text();
        return result;
    }

    query.bindValue(":group_value", group);

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << "Error executing query:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        QString key = query.value(0).toString();
        QString value = query.value(1).toString();
        result.insert(key, value);
    }

    return result;
}

