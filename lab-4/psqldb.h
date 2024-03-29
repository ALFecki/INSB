#ifndef PSQLDB_H
#define PSQLDB_H

#include <QDebug>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

struct User {
    int id;
    QString name;
    QString password;
    int role_id;

    User(int id, QString name, QString password, int role_id)
        : id(id), name(name), password(password), role_id(role_id) {
    }
};

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
    QList<User> getAllUsers();
    bool updateRow(int id, QString field, QString value);

private:
    QSqlDatabase* db;
};

#endif  // PSQLDB_H
