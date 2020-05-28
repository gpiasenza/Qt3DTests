#ifndef SCENE_H
#define SCENE_H

#include <QMap>
#include <QList>
#include <QString>
#include <Qt3DRender/QSceneLoader>

class Actor;

class Scene: public QObject
{
    Q_OBJECT

public:
    Scene(const char *sceneName, const char *fileName, Qt3DCore::QEntity *rootEntity);
    Actor *GetActor(const char *actorName);

protected:
    Qt3DCore::QEntity *rootEntity;
    QMap<QString, Actor *> actorMap;
    QList<Actor *> actorList;

    void AddEntity(Qt3DCore::QEntity *entity, Actor *parent, int level);

protected slots:
    void OnLoaderStatusChanged(Qt3DRender::QSceneLoader::Status status);

signals:
    void LoadCompleted();
};

#endif // SCENE_H
