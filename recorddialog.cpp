#include "recorddialog.hpp"
#include <ui_recorddialog.h>


namespace {
	QString timeFormat(qint64 time) {
		return QString::number(time / 1000) + " s " + QString::number(time % 1000) + " ms";
	}
}// namespace

RecordDialog::RecordDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RecordDialog), elapsedTimer(new QElapsedTimer) {
	ui->setupUi(this);
	ui->stopButton->setEnabled(false);
	connect(ui->startButton, &QPushButton::clicked, [&]() { startRecord(); });
	connect(ui->stopButton, &QPushButton::clicked, [&]() { stopRecord(); });
}

RecordDialog::~RecordDialog() {
	delete ui;
}

void RecordDialog::startRecord() {
	ui->stopButton->setEnabled(true);
	ui->startButton->setEnabled(false);
	//TODO: create temporary directory with unexisting name
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, [&]() { shot(); });
	elapsedTimer->start();
	timer->start(INTERVAL);
}

void RecordDialog::shot() {
	//TODO: take a screenshot and write it to created dir
	qint64 time = elapsedTimer->elapsed();
	ui->recordLabel->setText("Recording: " + timeFormat(time));
	if(time >= LIMIT) {
		stopRecord();
	}
}

void RecordDialog::stopRecord() {
	timer->stop();
	ui->stopButton->setEnabled(false);
	saveVideo();
	this->close();
}

void RecordDialog::saveVideo() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal Video"), "", tr("Video Files(*.mp4 *.avi);;All Files (*)"));
    if(!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        if(fileInfo.exists() && !fileInfo.isWritable()) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     "Can't save to " + fileInfo.fileName());
            return;
        }
       //TODO: start ffmpeg passing filename to exec(), it will perform writing
    }
}
