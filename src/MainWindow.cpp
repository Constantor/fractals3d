#include "MainWindow.hpp"

typedef QString string;
namespace {
	int getValFromBox(QDoubleSpinBox *box, QSlider *bar) {
		return (bar->maximum() - bar->minimum()) * (box->value() - box->minimum()) / (box->maximum() - box->minimum());
	}

	double getValFromBar(QDoubleSpinBox *box, QSlider *bar) {
		return box->minimum() + (box->maximum() - box->minimum()) * (bar->value() - bar->minimum()) / (bar->maximum() - bar->minimum());
	}

	QString timeFormat(qint64 time) {
		return QString::number(time / 1000) + " s " + QString::number(time % 1000) + " ms";
	}

	void saveImageToFile(const QImage &image, const QString &fileName) {
		image.save(fileName);
	}
}// namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->recordWidget->close();
	prevSize = this->size();
	connectBoxBar();
	connect(ui->recordButton, &QPushButton::clicked, [&]() { recordClickAction(); });
	setValues();
	updateButtons();
	ui->fractalWidget->setFractalData(&data);
	readAndDraw();
	makeMenu();
	hideBorders();
}

MainWindow::~MainWindow() {
	delete ui;
	delete timer;
	delete elapsedTimer;
	delete temporaryDir;
}

void MainWindow::makeMenu() {
	auto fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction("Load fractal", [&]() { loadFromFile(); });
	fileMenu->addAction("Save fractal", [&]() { saveToFile(); });
	fileMenu->addAction("Save as image", [&]() { saveToImage(); });
	fileMenu->addAction("Save as video", [&]() { recordVideo(); });
	fileMenu->addAction("Exit", [&]() { QApplication::quit(); });

	auto settingsMenu = menuBar()->addMenu("Settings");
	settingsMenu->addAction(
			"Fullscreen view ", [&]() { hideAndShow(); }, QKeySequence(tr("F11")));
	auto aboutMenu = menuBar()->addMenu("Help");
	aboutMenu->addAction("About", [&]() { QMessageBox::information(this, tr("About"), "Fractals 3D is a program intended for 3D fractals exploration. Done by the three students of the HSE University campus in Saint-Petersburg: Sergey Zhuravlev, Stepan Konstantinov, Daria Ledneva. Mentor: Anton Sosnin."); });
}

void MainWindow::updateButtons() {
	ui->fractalColorButton->setPalette(QPalette(data.fractalColor));
	ui->fractalColorButton->setText(data.fractalColor.name());
	ui->ambienceColorButton->setPalette(QPalette(data.ambienceColor));
	ui->ambienceColorButton->setText(data.ambienceColor.name());
}

void MainWindow::chooseColor(QColor const &color, ColorType type = FRACTAL) {
	QColor *colorMemory;
	if(type == AMBIENCE)
		colorMemory = &data.ambienceColor;
	else
		colorMemory = &data.fractalColor;
	if(color.isValid()) {
		*colorMemory = color;
		updateButtons();
		readAndDraw();
	}
}

void MainWindow::askColor(ColorType type) {
	QString title;
	if(type == AMBIENCE)
		title = "Select ambience color";
	else
		title = "Select fractal color";
	chooseColor(QColorDialog::getColor(Qt::green, this, title), type);
}

void MainWindow::connectBoxBar() {
	connect(ui->firstCoordBox, &QDoubleSpinBox::valueChanged, ui->firstCoordBar, [&]() { ui->firstCoordBar->setValue(getValFromBox(ui->firstCoordBox, ui->firstCoordBar)); });
	connect(ui->firstCoordBar, &QSlider::valueChanged, ui->firstCoordBox, [&]() { ui->firstCoordBox->setValue(getValFromBar(ui->firstCoordBox, ui->firstCoordBar)); });
	connect(ui->secondCoordBox, &QDoubleSpinBox::valueChanged, ui->secondCoordBar, [&]() { ui->secondCoordBar->setValue(getValFromBox(ui->secondCoordBox, ui->secondCoordBar)); });
	connect(ui->secondCoordBar, &QSlider::valueChanged, ui->secondCoordBox, [&]() { ui->secondCoordBox->setValue(getValFromBar(ui->secondCoordBox, ui->secondCoordBar)); });
	connect(ui->thirdCoordBox, &QDoubleSpinBox::valueChanged, ui->thirdCoordBar, [&]() { ui->thirdCoordBar->setValue(getValFromBox(ui->thirdCoordBox, ui->thirdCoordBar)); });
	connect(ui->thirdCoordBar, &QSlider::valueChanged, ui->thirdCoordBox, [&]() { ui->thirdCoordBox->setValue(getValFromBar(ui->thirdCoordBox, ui->thirdCoordBar)); });
	connect(ui->powerSpinBox, &QSpinBox::valueChanged, ui->powerBarSlider, [&]() { ui->powerBarSlider->setValue(ui->powerSpinBox->value() / 2); });
	connect(ui->powerBarSlider, &QSlider::valueChanged, ui->powerSpinBox, [&]() { ui->powerSpinBox->setValue(2 * ui->powerBarSlider->value()); });
	connect(ui->firstCoordBox, &QDoubleSpinBox::valueChanged, [&]() { readAndDraw(); });
	connect(ui->firstCoordBar, &QSlider::valueChanged, [&]() { readAndDraw(); });
	connect(ui->secondCoordBox, &QDoubleSpinBox::valueChanged, [&]() { readAndDraw(); });
	connect(ui->secondCoordBar, &QSlider::valueChanged, [&]() { readAndDraw(); });
	connect(ui->thirdCoordBox, &QDoubleSpinBox::valueChanged, [&]() { readAndDraw(); });
	connect(ui->thirdCoordBar, &QSlider::valueChanged, [&]() { readAndDraw(); });
	connect(ui->powerSpinBox, &QSpinBox::valueChanged, [&]() { readAndDraw(); });
	connect(ui->powerBarSlider, &QSlider::valueChanged, [&]() { readAndDraw(); });
	connect(ui->typeBox, &QComboBox::currentIndexChanged, [&]() { readAndDraw(); });
	connect(ui->fractalColorButton, &QPushButton::clicked, [&]() { askColor(FRACTAL); });
	connect(ui->ambienceColorButton, &QPushButton::clicked, [&]() { askColor(AMBIENCE); });
	connect(ui->randomizeButton, &QPushButton::clicked, [&]() { generateRandom(); });
	connect(ui->rotationBox, &QCheckBox::clicked, [&]() { readAndDraw(); });
}

void MainWindow::readAndDraw() {
	if(!isSetting) {
		data = FractalData(ui->firstCoordBox->value(), ui->secondCoordBox->value(), ui->thirdCoordBox->value(), ui->powerSpinBox->value(),
						   static_cast<FractalType>(ui->typeBox->currentIndex()), data.fractalColor, data.ambienceColor, data.camera, data.zoomCoefficient, ui->rotationBox->isChecked());
		ui->fractalWidget->repaint();
	}
}

void MainWindow::loadFromFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Fractal"), "", tr("3D Fractal Data (*.f3d);;All Files (*)"));
	if(fileName.isEmpty())
		return;
	else {
		QFile file(fileName);

		if(!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
									 file.errorString());
			return;
		}

		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		data.readFrom(doc);
		file.close();

		setValues();
		readAndDraw();
	}
}

void MainWindow::saveToFile() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save fractal configuration"), "", tr("3D Fractal Data (*.f3d);;All files (*.*)"));
	if(!fileName.isEmpty()) {
		QFile file(fileName);

		if(!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to save the file"), file.errorString());
			return;
		}

		QJsonObject output;
		output.insert("Fractal", data.serialize());

		QJsonDocument doc(output);
		file.write(doc.toJson());
		file.close();
	}
}

void MainWindow::saveToImage() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save fractal image"), "", tr("Image files (*.png *.jpg *.jpeg *.bmp)"));
	if(!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		if(fileInfo.exists() && !fileInfo.isWritable()) {
			QMessageBox::information(this, tr("Unable to write to the file"),
									 "Failed to save fractal data to " + fileInfo.fileName());
			return;
		}
		saveImageToFile(ui->fractalWidget->grabFramebuffer(), fileName);
	}
}

void MainWindow::setValues() {
	isSetting = true;
	ui->firstCoordBox->setValue(data.a);
	ui->secondCoordBox->setValue(data.b);
	ui->thirdCoordBox->setValue(data.c);
	ui->powerSpinBox->setValue(data.n);
	ui->typeBox->setCurrentIndex(data.type);
	updateButtons();
	isSetting = false;
}

void MainWindow::recordVideo() {
	ui->recordLabel->setText("Recording is not started.");
	ui->recordProgressBar->setValue(0);
	ui->recordWidget->show();
}

void MainWindow::startRecord() {
	isOnRecord = true;
	ui->recordButton->setText("Stop");
	time = 0, frames = 0;
	temporaryDir = new QTemporaryDir;
	recorder = new Recorder(Recorder::Mode::kWaitProcessing);
	recorder->start();
	timer = new QTimer(this);
	elapsedTimer = new QElapsedTimer;
	connect(timer, &QTimer::timeout, [&]() { shot(); });
	elapsedTimer->start();
	timer->start();
}

void MainWindow::shot() {
	time = elapsedTimer->elapsed();
	string fileName = QStringLiteral("%1.png").arg(time, 5, 10, QLatin1Char('0'));
	recorder->push_back({ui->fractalWidget->grabFramebuffer(), temporaryDir->filePath(fileName)});
	frames++;
	ui->recordLabel->setText("Recording: " + timeFormat(time));
	ui->recordProgressBar->setValue(100 * time / LIMIT);
	if(LIMIT <= time)
		stopRecord();
}

void MainWindow::stopRecord() {
	timer->stop();
	isOnRecord = false;
	saveVideo();
	ui->recordWidget->close();
}

void MainWindow::saveVideo() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal Video"), "", tr("Video Files(*.mp4 *.avi);;All Files (*)"));
	if(!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		if(fileInfo.exists() && !fileInfo.isWritable()) {
			QMessageBox::information(this, tr("Unable to write to the file"),
									 "Can't save to " + fileInfo.fileName());
			return;
		}
		//int framerate = frames * 1000 / time;
		QString command = QString("ffmpeg -y -pattern_type glob -i '%1/*.png' -c:v libx264 -r 60 -pix_fmt yuv420p -vf \"crop=trunc(iw/2)*2:trunc(ih/2)*2\" %2").arg(temporaryDir->path(), fileName);
		std::system(command.toStdString().data());
	}
}

void MainWindow::recordClickAction() {
	if(isOnRecord)
		stopRecord();
	else
		startRecord();
}

void MainWindow::generateRandom() {
	data.genRandom();
	setValues();
	ui->fractalWidget->repaint();
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_F11) {
		hideAndShow();
	}
	if(event->key() == Qt::Key_Escape && isFullScreen) {
		hideAndShow();
	}
	QWidget::keyPressEvent(event);
}

void MainWindow::hideAndShow() {
	if(isFullScreen) {
		ui->menubar->show();
		ui->inputWidget->show();
		ui->statusbar->show();
		this->resize(prevSize);
		isFullScreen = false;
	} else {
		prevSize = this->size();
		ui->menubar->hide();
		ui->inputWidget->hide();
		ui->statusbar->hide();
		this->showMaximized();
		isFullScreen = true;
	}
}

void MainWindow::hideBorders() {
	ui->statusbar->hide();
	ui->centralwidget->setContentsMargins(0, 0, 0, 0);
}
