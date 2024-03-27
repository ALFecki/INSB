#ifndef PSQLDB_H
#define PSQLDB_H

#include <QDebug>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class PSQLDBHelper {
public:
    PSQLDBHelper(const char* driver);
    ~PSQLDBHelper();
    bool connectToDatabase(const QString& server,
                           const QString& databaseName,
                           const QString& userName,
                           const QString& password);
    void disconnect();

    std::optional<QSqlQuery*> executeQuery(QSqlQuery* query);
    int getUserRole(QString login, QString password);

private:
    QSqlDatabase* db;
};

#endif  // PSQLDB_H
