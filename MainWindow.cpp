#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QScreen>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Create OpenGL view
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    // Set background colour
    view->defaultFrameGraph()->setClearColor(QRgb(0x1d1d4f));
    // Assign the root of the tree to the view
    view->setRootEntity(rootEntity);

    // Create containing QWidget
    container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);
    container->setFocusPolicy(Qt::NoFocus);

    setCentralWidget(container);

    // Camera
    cameraEntity = view->camera();

    // For the Blender model:
    // X+ -> right
    // Y+ -> away
    // Z+ -> up
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, -5, 5.0f));
    cameraEntity->setUpVector(QVector3D(0, 0, 1));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Create and place a light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(-20, 20, 20));
    lightEntity->addComponent(lightTransform);

    // Import the scene
    sceneLoaderEntity = new Qt3DCore::QEntity(rootEntity);
    loader = new Qt3DRender::QSceneLoader(sceneLoaderEntity);
    loader->setObjectName("object1.dae");
    sceneLoaderEntity->addComponent(loader);
    loader->setSource(QUrl(QString("file:../Qt3DTests/Models/object1.dae")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

