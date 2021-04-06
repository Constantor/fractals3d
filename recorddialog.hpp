#pragma once

#include <QDialog>
#include <ui_recorddialog.h>
#include <QPushButton>
#include <QTimer>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RecordDialog;
}
QT_END_NAMESPACE

class RecordDialog: public QDialog{
Q_OBJECT

    static const qint64 INTERVAL = 40;
    static const qint64 LIMIT = 10000;

public:
    explicit RecordDialog(QWidget *parent = nullptr);

    ~RecordDialog() override;

private:
    Ui::RecordDialog *ui;
	QElapsedTimer* elapsedTimer;
	QTimer* timer;

	void startRecord();
	void shot();
	void stopRecord();
};