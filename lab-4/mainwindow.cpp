#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    login = new LoginWindow();
    this->hide();
    login->show();
    centralWidget()->setParent(0);
    this->setCentralWidget(login);
}

MainWindow::~MainWindow() {
    delete ui;
}
