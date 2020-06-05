/*****************************************************************************
 *
 * This program is provided as an example of Qt3D visualisation using C++.
 * It complements a series of articles I'm writing on Linkedin.
 *
 * You can use it and modify it in any way you find suitable, without
 * any specific license.
 *
 * It comes without any guarantee, and it's not supposed to be a model of
 * programming best practices. In fact, most best practices are intentionally
 * omitted (e.g. return values are not checked, pointers are not checked,
 * memory is not freed) for the purpose of keeping code short to illustrate
 * fundamental concepts of Qt3D.
 * It has very no comments, because it's supposed to be read together with
 * the article.
 *
 * That being said, I hope you can find the example useful.
 *
 * You can find my tutorials on my Linkedin account:
 * https://www.linkedin.com/in/guido-piasenza-60689418/
 *
 * https://www.soft-in.com/
 *
 ****************************************************************************/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "EntityWrapper.h"

#include <QScreen>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <math.h>


// ./Qt3DTests > tree.txt
// dot -Tpdf tree.txt | okular -
int Tree(Qt3DCore::QEntity *e, int &nextNodeNumber)
{
    int myNodeNumber = nextNodeNumber++;
    int childNodeNumber;

    // Insert myself in the tree
    printf("n%03d ;\n", myNodeNumber);
    printf("n%03d [label=\"%s\n%s\"] ;\n",
           myNodeNumber,
           e->metaObject()->className(),
           e->objectName().toStdString().c_str());

    // Enumerate components
    for(Qt3DCore::QComponent *comp: e->components())
    {
        printf("n%03d ;\n", nextNodeNumber);
        printf("n%03d [shape=box,label=\"%s\n%s\"] ;\n",
               nextNodeNumber,
               comp->metaObject()->className(),
               comp->objectName().toStdString().c_str());
        printf("n%03d -- n%03d [style=dotted];\n", myNodeNumber, nextNodeNumber);
        nextNodeNumber++;
    }


    // Build tree for children
    for (auto childNode : e->childNodes())
    {
        auto childEntity = qobject_cast<Qt3DCore::QEntity *>(childNode);
        if (childEntity)
        {
            childNodeNumber = Tree(childEntity, nextNodeNumber);
            printf("n%03d -- n%03d ;\n", myNodeNumber, childNodeNumber);
        }
    }

    return myNodeNumber;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Root entity
    rootEntity = new Qt3DCore::QEntity();
    rootEntity->setObjectName("This is the root");

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
    QObject::connect(loader, &Qt3DRender::QSceneLoader::statusChanged, this, &MainWindow::OnLoaderStatusChanged);

    connect(&timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    timer.start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnLoaderStatusChanged(Qt3DRender::QSceneLoader::Status status)
{
    if(status == Qt3DRender::QSceneLoader::Ready)
    {
        printf("graph \"\"\n");
        printf("{\n");
        printf("label=\"Entity tree\"\n");
        // printf("rankdir=LR\n");
        int rootNodeNumber=0;
        Tree(rootEntity, rootNodeNumber);
        printf("}\n");

        auto torusEntity = rootEntity->findChild<Qt3DCore::QEntity *>("TorusObject");
        if(torusEntity)
            torusWrapper = new EntityWrapper(torusEntity);
    }
}

void MainWindow::OnTimer()
{
    if(torusWrapper)
    {
        alpha += 0.01;
        torusWrapper->Move(cos(alpha), sin(alpha), 0);
        torusWrapper->setSpecular(QColor(colourValue, 0, 255-colourValue));
        colourValue += colourDir;
        if(colourValue == 0 || colourValue == 255)
            colourDir = - colourDir;
    }
}
