#include "refactordialog.h"

refactorDialog::refactorDialog(QWidget *parent) : QDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *label1 = new QLabel("Row:", this);
  row = new QSpinBox(this);
  layout->addWidget(label1);
  layout->addWidget(row);

  QLabel *label2 = new QLabel("Column:", this);
  column = new QSpinBox(this);
  layout->addWidget(label2);
  layout->addWidget(column);

  QLabel *label3 = new QLabel("Value:", this);
  value = new QLineEdit(this);
  layout->addWidget(label3);
  layout->addWidget(value);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  layout->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

std::tuple<int, int, QString> refactorDialog::getValues() {
  return std::tuple<int, int, QString>(row->value(), column->value(),
                                       value->text());
}
