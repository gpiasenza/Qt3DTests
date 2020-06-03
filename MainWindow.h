#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <Qt3DRender/QCamera>
#include <Qt3DRender/QSceneLoader>

#include <Qt3DCore/QEntity>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void OnLoaderStatusChanged(Qt3DRender::QSceneLoader::Status status);
    void OnTimer();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    double alpha = 0;

    QWidget *container;

    Qt3DCore::QEntity *rootEntity = nullptr;
    Qt3DRender::QCamera *cameraEntity = nullptr;
    Qt3DCore::QEntity *sceneLoaderEntity = nullptr;
    Qt3DRender::QSceneLoader *loader = nullptr;
    Qt3DCore::QTransform *transform = nullptr;
};

#endif // MAINWINDOW_H
