#include "loginwindow.h"

LoginWindow::LoginWindow(PSQLDBHelper* dbConnection, QWidget* parent)
    : QMainWindow(parent), dbConnection(dbConnection), ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    ui->login_le->setPlaceholderText("Login");
    ui->login_le->setMaxLength(35);
    ui->password_le->setMaxLength(35);
    ui->password_le->setPlaceholderText("Password");
    ui->password_le->setEchoMode(QLineEdit::Password);
    connect(this->ui->pushButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked() {
    QString login = this->ui->login_le->text(), password = this->ui->password_le->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Error", "Fill all fields, please");
        return;
    }

    auto priviledge_level = dbConnection->getUserRole(login, password);
    if (priviledge_level == -1) {
        QMessageBox::critical(this, "Error", "Incorrect login or password!");
        return;
    }
    qDebug() << "User found, success authorization";
    emit success(priviledge_level);
}

void LoginWindow::clearInputs() {
    this->ui->login_le->clear();
    this->ui->password_le->clear();
}

LoginWindow::~LoginWindow() {
    qDebug() << "Login desctructor called!";
    delete ui;
}
