#pragma once

#include <QDialog>
#include <ui_recorddialog.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RecordDialog;
}
QT_END_NAMESPACE

class RecordDialog: public QDialog{
Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);

    ~RecordDialog() override;

private:
    Ui::RecordDialog *ui;

};