#include "recorddialog.hpp"
#include <ui_recorddialog.h>

RecordDialog::RecordDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RecordDialog){
    ui->setupUi(this);
}
RecordDialog::~RecordDialog() {
    delete ui;
}
