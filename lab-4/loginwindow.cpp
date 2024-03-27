#include "loginwindow.h"

#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    ui->login_te->setPlaceholderText("Login");
    ui->password_te->setPlaceholderText("Password");
}

LoginWindow::~LoginWindow() {
    delete ui;
}
