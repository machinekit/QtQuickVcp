#include "qpathview3d.h"

QPathView3d::QPathView3d() :
    QGLView()
{
    QGLCubeItem *cubeItem;
    cubeItem = new QGLCubeItem();
    cubeItem->setPosition(0,0,-7);
    cubeItem->setColor(Qt::red);
    addItem(cubeItem);

    QGLSphereItem *sphereItem = new QGLSphereItem();
    sphereItem->setPosition(1,2,-7);
    sphereItem->setColor(Qt::blue);
    addItem(sphereItem);

    QGLGridItem *gridItem = new QGLGridItem();
    gridItem->setPosition(0,0,-7);
    addItem(gridItem);
}
