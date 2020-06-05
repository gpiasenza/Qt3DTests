#ifndef ENTITYWRAPPER_H
#define ENTITYWRAPPER_H


namespace Qt3DCore
{
    class QEntity;
    class QTransform;
}

namespace Qt3DExtras
{
    class QPhongMaterial;
}

#include <QMatrix4x4>
#include <QList>
#include <QMap>

class EntityWrapper
{
public:
    EntityWrapper(Qt3DCore::QEntity *entity);

    void Move(double x, double y, double z);
    void setSpecular(const QColor &col);

protected:
    Qt3DCore::QEntity *entity = nullptr;
    Qt3DCore::QTransform *transform = nullptr;
    Qt3DExtras::QPhongMaterial *mat = nullptr;
    QList<EntityWrapper *> children;
};

#endif // ENTITYWRAPPER_H
