#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      dbConnection(PSQLDBHelper("QPSQL")),
      login(LoginWindow(&dbConnection)) {
    ui->setupUi(this);
    if (!dbConnection.connectToDatabase(DB_HOST, DB_NAME, DB_USER, DB_PASSWORD)) {
        qDebug() << "Cannot connect to database!";
    }
    qDebug() << "Connection with database established!";
    login.show();

    centralWidget()->setParent(nullptr);
    this->setCentralWidget(&login);

    this->ui->usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(&this->login, SIGNAL(success(int)), this, SLOT(successLogin(int)));
    connect(this->ui->logOut, &QPushButton::clicked, this, &MainWindow::logout);
    connect(this->ui->refactorButton, &QPushButton::clicked, this, &MainWindow::refactor);
}

void MainWindow::fillTableView(QStandardItemModel *model, Priviligies role) {
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Name");
    if (role == Priviligies::ADMIN || role == Priviligies::STAFF) {
        model->setHeaderData(2, Qt::Horizontal, "Role ID");
    }
    if (role == Priviligies::ADMIN) {
        model->setHeaderData(3, Qt::Horizontal, "Password");
    }

    auto users = this->dbConnection.getAllUsers();
    for (size_t i = 0; i < users.size(); ++i) {
        model->setData(model->index(i, 0), users[i].id);
        model->setData(model->index(i, 1), users[i].name);

        if (role == Priviligies::ADMIN || role == Priviligies::STAFF) {
            model->setData(model->index(i, 2), users[i].role_id);
        }

        if (role == Priviligies::ADMIN) {
            model->setData(model->index(i, 3), users[i].password);
        }
    }
}

void MainWindow::successLogin(int lvl) {
    qDebug() << "Priviledge level: " << lvl;

    this->login.clearInputs();
    this->login.hide();
    centralWidget()->setParent(nullptr);
    this->setCentralWidget(this->ui->centralwidget);
    qDebug() << &this->login;
    auto users = this->dbConnection.getAllUsers();
    QStandardItemModel *model = nullptr;
    switch (lvl) {
        case Priviligies::USER: {
            model = new QStandardItemModel(users.size(), 2);
            this->ui->refactorButton->hide();
            this->ui->logOut->show();
            break;
        }
        case Priviligies::STAFF: {
            model = new QStandardItemModel(users.size(), 3);
            this->ui->refactorButton->hide();
            this->ui->logOut->show();
            break;
        }
        case Priviligies::ADMIN: {
            model = new QStandardItemModel(users.size(), 4);
            this->ui->refactorButton->show();
            this->ui->logOut->show();
            break;
        }
        default: {
            QMessageBox::critical(this, "Access denied", "You have no needed role!");
            this->ui->refactorButton->hide();
            this->ui->usersTable->hide();
            this->ui->logOut->show();
            return;
        }
    }
    fillTableView(model, Priviligies(lvl));
    this->ui->usersTable->setModel(model);
    this->ui->usersTable->show();
}

void MainWindow::refactor() {
    refactorDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        auto users = this->dbConnection.getAllUsers();
        auto res = dialog.getValues();
        int row = std::get<0>(res), column = std::get<1>(res);
        QString value = std::get<2>(res);
        if (value.isEmpty()) {
            QMessageBox::warning(this, "Error", "Fill the value field!");
            return;
        }
        if (value.size() > 25) {
            QMessageBox::warning(this, "Error", "The value is too long!");
            return;
        }
        if (row >= users.size() || column >= 4) {
            QMessageBox::warning(this, "Error", "Invalid row or column number!");
            return;
        }
        if ((column == 0 || column == 2) && value.contains(QRegularExpression("[a-zA-Z]"))) {
            QMessageBox::warning(this, "Error", "In this column only numbers are available!");
            return;
        }
    }
}

void MainWindow::logout() {
    qDebug() << &this->login;
    centralWidget()->setParent(nullptr);
    this->setCentralWidget(&this->login);
    this->login.show();
}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow destructor called!";
    dbConnection.disconnect();
    //    delete login;
    delete ui;
}
