#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), dbConnection(PSQLDBHelper("QPSQL")) {
    ui->setupUi(this);
    if (!dbConnection.connectToDatabase(DB_HOST, DB_NAME, DB_USER, DB_PASSWORD)) {
        qDebug() << "Cannot connect to database!";
    }
    qDebug() << "Connection with database established!";

    login = new LoginWindow(&dbConnection);
    this->hide();
    login->show();

    centralWidget()->setParent(nullptr);
    this->setCentralWidget(login);

    connect(this->login, SIGNAL(success(int)), this, SLOT(successLogin(int)));
}

void MainWindow::successLogin(int lvl) {
    qDebug() << "Priviledge level: " << lvl;
    this->login->hide();
    auto users = this->dbConnection.getAllUsers();
    switch (lvl) {
        case Priviligies::USER: {
            this->ui->refactorButton->hide();
        }
    }

    this->show();
}

MainWindow::~MainWindow() {
    dbConnection.disconnect();
    delete ui;
}
