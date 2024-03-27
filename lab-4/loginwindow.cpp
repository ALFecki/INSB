#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), dbConnection(PSQLDBHelper("QPSQL")), ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    ui->login_le->setPlaceholderText("Login");
    ui->login_le->setMaxLength(25);
    ui->password_le->setMaxLength(25);
    ui->password_le->setPlaceholderText("Password");
    ui->password_le->setEchoMode(QLineEdit::Password);
    connect(this->ui->pushButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    if (!dbConnection.connectToDatabase(DB_HOST, DB_NAME, DB_USER, DB_PASSWORD)) {
        qDebug() << "Cannot connect to database!";
    }
}

void LoginWindow::onLoginClicked() {
    QString login = this->ui->login_le->text(), password = this->ui->password_le->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Error", "Fill all fields, please");
        return;
    }

    auto priviledge_level = dbConnection.getUserRole(login, password);
    if (priviledge_level == -1) {
        QMessageBox::critical(this, "Error", "Incorrect login or password!");
        return;
    }
    emit successLogin(priviledge_level);
}

LoginWindow::~LoginWindow() {
    delete ui;
}
