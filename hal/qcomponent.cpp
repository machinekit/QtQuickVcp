#include "qcomponent.h"

QComponent::QComponent(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_context = NULL;
    m_updateSocket = NULL;
    m_cmdSocket = NULL;
    m_instanceCount = 0;
}

void QComponent::componentComplete()
{
    QObjectList halObjects;

    halObjects = recurseObjects(this->children());
    foreach (QObject *object, halObjects)
    {
        qDebug() << object->property("name");
    }

    QQuickItem::componentComplete();
}

QObjectList QComponent::recurseObjects(const QObjectList &list)
{
    QObjectList halObjects;

    foreach (QObject *object, list)
    {
        if (object->inherits("QPin"))
        {
            halObjects.append(object);
        }

        if (object->children().size() > 0)
        {
            halObjects.append(recurseObjects(object->children()));
        }
    }

    return halObjects;
}

