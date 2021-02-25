#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QScreen>

#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);

	Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
	view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
	QWidget * container = QWidget::createWindowContainer(view);
	QSize screenSize = view->screen()->size();
	container->setMinimumSize(QSize(200, 100));
	container->setMaximumSize(screenSize);

	QWidget * widget = new QWidget;
	QHBoxLayout * hLayout = new QHBoxLayout(widget);
	QVBoxLayout * vLayout = new QVBoxLayout();
	vLayout->setAlignment(Qt::AlignTop);
	hLayout->addWidget(container, 1);
	hLayout->addLayout(vLayout);

	widget->setWindowTitle(QStringLiteral("Basic shapes"));

	// Root entity
	Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

	// Camera
	Qt3DRender::QCamera *cameraEntity = view->camera();

	cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
	cameraEntity->setUpVector(QVector3D(0, 1, 0));
	cameraEntity->setViewCenter(QVector3D(0, 0, 0));

	Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
	Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
	light->setColor("white");
	light->setIntensity(1);
	lightEntity->addComponent(light);
	Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
	lightTransform->setTranslation(cameraEntity->position());
	lightEntity->addComponent(lightTransform);

	// For camera controls
	Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
	camController->setCamera(cameraEntity);

	// Set root object of the scene
	view->setRootEntity(rootEntity);

	Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();

	// CuboidMesh Transform
	Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
	cuboidTransform->setScale(4.0f);
	cuboidTransform->setTranslation(QVector3D(5.0f, -4.0f, 0.0f));

	Qt3DExtras::QPhongMaterial *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
	cuboidMaterial->setDiffuse(QColor(QRgb(0x123456)));

	// Cuboid
	Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(rootEntity);
	cuboidEntity->addComponent(cuboid);
	cuboidEntity->addComponent(cuboidMaterial);
	cuboidEntity->addComponent(cuboidTransform);
	cuboidEntity->setEnabled(true);

	// Show window
	widget->show();
	widget->resize(1200, 800);

	return fractals3DApp.exec();
}
