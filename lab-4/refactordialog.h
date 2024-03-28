#ifndef REFACTORDIALOG_H
#define REFACTORDIALOG_H

#include <QtWidgets>

class refactorDialog : public QDialog {
public:
    refactorDialog(QWidget *parent = nullptr);

    std::tuple<int, int, QString> getValues();

private:
    QSpinBox *row;
    QSpinBox *column;
    QLineEdit *value;
};

#endif  // REFACTORDIALOG_H
