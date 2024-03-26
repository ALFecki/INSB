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
    QSqlDatabase* connectToDatabase(const QString& server,
                                    const QString& databaseName,
                                    const QString& userName,
                                    const QString& password);
    void disconnect();

    //    int selectRowCountResult(QSqlQuery* query);
    bool executeQuery(QSqlQuery* query);
    //    bool executeUpdate(QSqlQuery* query);

private:
    QSqlDatabase* db;
};

#endif  // PSQLDB_H
