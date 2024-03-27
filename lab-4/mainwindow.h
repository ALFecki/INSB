#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "loginwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void successLogin(int);
    void logout();

private:
    Ui::MainWindow *ui;
    PSQLDBHelper dbConnection;
    LoginWindow *login;
};
#endif  // MAINWINDOW_H
