#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "psqldb.h"
#include "ui_loginwindow.h"

#define DB_HOST "localhost"
#define DB_NAME "lab-4"
#define DB_USER "root"
#define DB_PASSWORD "root"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow {
    Q_OBJECT

signals:
    void successLogin(int);

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    void onLoginClicked();
    PSQLDBHelper dbConnection;
    Ui::LoginWindow *ui;
};

#endif  // LOGINWINDOW_H
