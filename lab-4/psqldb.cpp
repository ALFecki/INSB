#include "psqldb.h"

PSQLDBHelper::PSQLDBHelper(const char* driver) {
    db = new QSqlDatabase(QSqlDatabase::addDatabase(driver));
}

bool PSQLDBHelper::connectToDatabase(const QString& server,
                                     const QString& databaseName,
                                     const QString& userName,
                                     const QString& password) {
    this->db->setConnectOptions();
    this->db->setHostName(server);
    this->db->setDatabaseName(databaseName);
    this->db->setUserName(userName);
    this->db->setPassword(password);

    return this->db->open();
}

std::optional<QSqlQuery*> PSQLDBHelper::executeQuery(QSqlQuery* query) {
    db->transaction();
    if (!query->exec() || query->lastError().type() != QSqlError::NoError || !query->first()) {
        qDebug() << query->lastError().text();
        db->rollback();
        return {};
    }
    db->commit();
    return query;
}

int PSQLDBHelper::getUserRole(QString login, QString password) {
    QSqlQuery query;
    query.prepare("SELECT role_id FROM user WHERE login = :login AND password = :password;");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    auto res = this->executeQuery(&query);
    if (res.has_value()) {
        long long role_id = res.value()->value(0).toInt();
        query.prepare("SELECT level FROM roles WHERE id = :id;");
        query.bindValue(":id", role_id);
        res = this->executeQuery(&query);
        if (res.has_value()) {
            return res.value()->value(0).toInt();
        }
    }
    return -1;
}

void PSQLDBHelper::disconnect() {
    qDebug() << "Disconnected from database!";
    db->close();
}

PSQLDBHelper::~PSQLDBHelper() {
    qDebug() << "Destructor called!";
    delete db;
}
