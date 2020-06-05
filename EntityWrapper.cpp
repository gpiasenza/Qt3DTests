#include "EntityWrapper.h"

#include <Qt3DCore/QComponent>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>

EntityWrapper::EntityWrapper(Qt3DCore::QEntity *entity)
{
    this->entity = entity;

    for(auto comp: entity->components())
    {
        if(!transform)
            transform = qobject_cast<Qt3DCore::QTransform *>(comp);

        if(!mat)
            mat = qobject_cast<Qt3DExtras::QPhongMaterial *>(comp);
    }

    for(auto childNode: entity->childNodes())
    {
        auto childEntity = qobject_cast<Qt3DCore::QEntity *>(childNode);
        if (childEntity)
            children.push_back(new EntityWrapper(childEntity));
    }
}

void EntityWrapper::Move(double x, double y, double z)
{
    if(transform)
        transform->setTranslation(QVector3D(x, y, z));
}

void EntityWrapper::setSpecular(const QColor &col)
{
    if(mat)
        mat->setSpecular(col);
    for(auto child: children)
        child->setSpecular(col);
}
