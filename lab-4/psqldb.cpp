#include "psqldb.h"

PSQLDBHelper::PSQLDBHelper(const char* driver) {
    db = new QSqlDatabase(QSqlDatabase::addDatabase(driver));
}

QSqlDatabase* PSQLDBHelper::connectToDatabase(const QString& server,
                                              const QString& databaseName,
                                              const QString& userName,
                                              const QString& password) {
    this->db->setConnectOptions();
    this->db->setHostName(server);
    this->db->setDatabaseName(databaseName);
    this->db->setUserName(userName);
    this->db->setPassword(password);

    if (this->db->open()) {
        return this->db;
    } else {
        return NULL;
    }
}

bool PSQLDBHelper::executeQuery(QSqlQuery* query) {
    db->transaction();
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes) {
        qDebug() << query->lastError().text();
        db->rollback();
        return false;
    }
    db->commit();
    return true;
}

void PSQLDBHelper::disconnect() {
    qDebug() << "Disconnected from database!";
    db->close();
}

PSQLDBHelper::~PSQLDBHelper() {
    qDebug() << "Destructor called!";
    delete db;
}
