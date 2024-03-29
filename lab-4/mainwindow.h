#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QStandardItemModel>
#include <QTableView>

#include "loginwindow.h"
#include "refactordialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void fillTableView(QStandardItemModel *, Priviligies);
    ~MainWindow();

private slots:
    void successLogin(int);
    void refactor();
    void logout();

private:
    Ui::MainWindow *ui;
    PSQLDBHelper dbConnection;
    LoginWindow login;
};
#endif  // MAINWINDOW_H
