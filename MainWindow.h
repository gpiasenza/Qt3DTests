#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    QWidget *container;

    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *cameraEntity;
    Qt3DCore::QEntity *sceneLoaderEntity;
    Qt3DRender::QSceneLoader *loader;
};

#endif // MAINWINDOW_H
