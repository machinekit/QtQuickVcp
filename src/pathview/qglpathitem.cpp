/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

#include "qglpathitem.h"
#include <QtCore/qmath.h>
#include "debughelper.h"

QGLPathItem::QGLPathItem(QQuickItem *parent) :
    QGLItem(parent),
    m_model(nullptr),
    m_arcFeedColor(QColor(Qt::white)),
    m_straightFeedColor(QColor(Qt::white)),
    m_traverseColor(QColor(Qt::cyan)),
    m_backplotArcFeedColor(QColor(Qt::yellow)),
    m_backplotStraightFeedColor(QColor(Qt::yellow)),
    m_backplotTraverseColor(QColor(Qt::yellow)),
    m_selectedColor(QColor(Qt::magenta)),
    m_activeColor(QColor(Qt::red)),
    m_traverseLineStippleLength(1.0),
    m_needsFullUpdate(true),
    m_minimumExtents(QVector3D(0, 0, 0)),
    m_maximumExtents(QVector3D(0, 0, 0))
{
    connect(this, SIGNAL(visibleChanged()),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(scaleChanged()),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(rotationChanged(QQuaternion)),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(rotationAngleChanged(float)),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(rotationAxisChanged(QVector3D)),
            this, SLOT(triggerFullUpdate()));
    connect(this, SIGNAL(visibleChanged()),
            this, SLOT(triggerFullUpdate()));
}

QGLPathItem::~QGLPathItem()
{
    qDeleteAll(m_previewPathItems);
}

void QGLPathItem::paint(QGLView *glView)
{
    if (m_needsFullUpdate)
    {
        glView->prepare(this);
        glView->reset();
        glView->beginUnion();

        for (int i = 0; i < m_previewPathItems.size(); ++i)
        {
            void* drawablePointer = nullptr;
            PathItem *pathItem = m_previewPathItems.at(i);
            if (pathItem->pathType == Line)
            {
                LinePathItem *linePathItem = static_cast<LinePathItem*>(pathItem);
                if (linePathItem->movementType == FeedMove)
                {
                    glView->color(m_straightFeedColor);
                }
                else
                {
                    glView->color(m_traverseColor);
                    glView->lineStipple(true, m_traverseLineStippleLength);
                }
                glView->translate(linePathItem->position);
                drawablePointer = glView->line(linePathItem->lineVector);
            }
            else if (pathItem->pathType == Arc)
            {
                ArcPathItem *arcPathItem = static_cast<ArcPathItem*>(pathItem);
                glView->color(m_arcFeedColor);
                glView->translate(arcPathItem->position);
                if (arcPathItem->rotationPlane == XZPlane) {
                    glView->rotate(90, 1, 0, 0);
                }
                else if  (arcPathItem->rotationPlane == YZPlane) {
                    glView->rotate(-90, 0, 1, 0);
                }
                drawablePointer = glView->arc(arcPathItem->center.x(),
                                              arcPathItem->center.y(),
                                              arcPathItem->radius,
                                              arcPathItem->startAngle,
                                              arcPathItem->endAngle,
                                              arcPathItem->anticlockwise,
                                              arcPathItem->helixOffset);
            }

            if (drawablePointer != nullptr)
            {
                pathItem->drawablePointer = drawablePointer;
                m_drawablePathMap.insert(drawablePointer, pathItem);
            }
        }

        glView->endUnion();

        m_needsFullUpdate = false;
    }
    else
    {
        for (int i = 0; i < m_modifiedPathItems.size(); ++i)
        {
            PathItem *pathItem;

            pathItem = m_modifiedPathItems.at(i);
            if (pathItem != nullptr)
            {
                QColor color;
                if (m_model->data(pathItem->modelIndex, QGCodeProgramModel::SelectedRole).toBool()) {
                    color = m_selectedColor;
                }
                else if (m_model->data(pathItem->modelIndex, QGCodeProgramModel::ActiveRole).toBool())
                {
                    color = m_activeColor;
                }
                else if (m_model->data(pathItem->modelIndex, QGCodeProgramModel::ExecutedRole).toBool())
                {
                    if (pathItem->movementType == FeedMove) {
                        if (pathItem->pathType == Arc) {
                            color = m_backplotArcFeedColor;
                        }
                        else {
                            color = m_backplotStraightFeedColor;
                        }
                    }
                    else {
                        color = m_backplotTraverseColor;
                    }
                }
                else
                {
                    if (pathItem->movementType == FeedMove) {
                        if (pathItem->pathType == Arc) {
                            color = m_arcFeedColor;
                        }
                        else {
                            color = m_straightFeedColor;
                        }
                    }
                    else {
                        color = m_traverseColor;
                    }
                }
                glView->updateColor(pathItem->drawablePointer, color);
            }
        }
        m_modifiedPathItems.clear();
    }
}

QGCodeProgramModel *QGLPathItem::model() const
{
    return m_model;
}

QColor QGLPathItem::arcFeedColor() const
{
    return m_arcFeedColor;
}

QColor QGLPathItem::traverseColor() const
{
    return m_traverseColor;
}

QColor QGLPathItem::selectedColor() const
{
    return m_selectedColor;
}

QVector3D QGLPathItem::minimumExtents() const
{
    return m_minimumExtents;
}

QVector3D QGLPathItem::maximumExtents() const
{
    return m_maximumExtents;
}

float QGLPathItem::traverseLineStippleLength() const
{
    return m_traverseLineStippleLength;
}

QColor QGLPathItem::straightFeedColor() const
{
    return m_straightFeedColor;
}

QColor QGLPathItem::activeColor() const
{
    return m_activeColor;
}

QColor QGLPathItem::backplotArcFeedColor() const
{
    return m_backplotArcFeedColor;
}

QColor QGLPathItem::backplotStraightFeedColor() const
{
    return m_backplotStraightFeedColor;
}

QColor QGLPathItem::backplotTraverseColor() const
{
    return m_backplotTraverseColor;
}

void QGLPathItem::selectDrawable(void *pointer)
{
    PathItem *mappedPathItem;
    QModelIndex mappedModelIndex;

    if (m_model == nullptr)
    {
        return;
    }

    mappedPathItem = m_drawablePathMap.value(pointer, nullptr);
    if (mappedPathItem != nullptr)
    {
        mappedModelIndex = mappedPathItem->modelIndex;
        m_model->setData(mappedModelIndex, true, QGCodeProgramModel::SelectedRole);
    }

    if (m_previousSelectedDrawable != pointer)
    {
        mappedPathItem = m_drawablePathMap.value(m_previousSelectedDrawable);
        if (mappedPathItem != nullptr)
        {
            mappedModelIndex = mappedPathItem->modelIndex;
            m_model->setData(mappedModelIndex, false, QGCodeProgramModel::SelectedRole);
        }

        m_previousSelectedDrawable = pointer;
    }
}

void QGLPathItem::setModel(QGCodeProgramModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged(arg);

        if (m_model != nullptr)
        {
            connect(m_model, SIGNAL(modelReset()),
                    this, SLOT(drawPath()));
            connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                    this, SLOT(modelDataChanged(QModelIndex,QModelIndex,QVector<int>)));

            if (m_model->rowCount() > 0)
            {
                drawPath();     // draw model when set
            }
        }
    }
}

void QGLPathItem::setArcFeedColor(QColor arg)
{
    if (m_arcFeedColor != arg) {
        m_arcFeedColor = arg;
        emit arcFeedColorChanged(arg);
    }
}

void QGLPathItem::setTraverseColor(QColor arg)
{
    if (m_traverseColor != arg) {
        m_traverseColor = arg;
        emit traverseColorChanged(arg);
    }
}

void QGLPathItem::setSelectedColor(QColor arg)
{
    if (m_selectedColor != arg) {
        m_selectedColor = arg;
        emit selectedColorChanged(arg);
    }
}

void QGLPathItem::setStraightFeedColor(QColor arg)
{
    if (m_straightFeedColor != arg) {
        m_straightFeedColor = arg;
        emit straightFeedColorChanged(arg);
    }
}

void QGLPathItem::setActiveColor(QColor arg)
{
    if (m_activeColor != arg) {
        m_activeColor = arg;
        emit activeColorChanged(arg);
    }
}

void QGLPathItem::setTraverseLineStippleLength(float arg)
{
    if (m_traverseLineStippleLength != arg) {
        m_traverseLineStippleLength = arg;
        emit traverseLineStippleLengthChanged(arg);
    }
}

void QGLPathItem::setBackplotArcFeedColor(QColor arg)
{
    if (m_backplotArcFeedColor != arg) {
        m_backplotArcFeedColor = arg;
        emit backplotArcFeedColorChanged(arg);
    }
}

void QGLPathItem::setBackplotStraightFeedColor(QColor arg)
{
    if (m_backplotStraightFeedColor != arg) {
        m_backplotStraightFeedColor = arg;
        emit backplotStraightFeedColorChanged(arg);
    }
}

void QGLPathItem::setBackplotTraverseColor(QColor arg)
{
    if (m_backplotTraverseColor != arg) {
        m_backplotTraverseColor = arg;
        emit backplotTraverseColorChanged(arg);
    }
}

void QGLPathItem::resetActiveOffsets()
{
    Position clearOffset;
    clearOffset.x = 0.0;
    clearOffset.y = 0.0;
    clearOffset.z = 0.0;
    clearOffset.a = 0.0;
    clearOffset.b = 0.0;
    clearOffset.c = 0.0;
    clearOffset.u = 0.0;
    clearOffset.v = 0.0;
    clearOffset.w = 0.0;

    m_activeOffsets.g92Offset = clearOffset;
    m_activeOffsets.toolOffset = clearOffset;
    m_activeOffsets.g5xOffsets.clear();
    for (int i = 0; i < 9; ++i)
    {
        m_activeOffsets.g5xOffsets.append(clearOffset);
    }
    m_activeOffsets.g5xOffsetIndex = 1;
}

void QGLPathItem::resetCurrentPosition()
{
    m_currentPosition.x = 0.0;
    m_currentPosition.y = 0.0;
    m_currentPosition.z = 0.0;
    m_currentPosition.a = 0.0;
    m_currentPosition.b = 0.0;
    m_currentPosition.c = 0.0;
    m_currentPosition.u = 0.0;
    m_currentPosition.v = 0.0;
    m_currentPosition.w = 0.0;
}

void QGLPathItem::resetActivePlane()
{
    m_activePlane = XYPlane;
}

void QGLPathItem::resetExtents()
{
    m_maximumExtents = QVector3D();
    m_minimumExtents = QVector3D();
}

void QGLPathItem::updateExtents(const QVector3D &vector)
{
    if (vector.x() < m_minimumExtents.x()) {
        m_minimumExtents.setX(vector.x());
    }
    if (vector.y() < m_minimumExtents.y()) {
        m_minimumExtents.setY(vector.y());
    }
    if (vector.z() < m_minimumExtents.z()) {
        m_minimumExtents.setZ(vector.z());
    }
    if (vector.x() > m_maximumExtents.x()) {
        m_maximumExtents.setX(vector.x());
    }
    if (vector.y() > m_maximumExtents.y()) {
        m_maximumExtents.setY(vector.y());
    }
    if (vector.z() > m_maximumExtents.z()) {
        m_maximumExtents.setZ(vector.z());
    }
}

void QGLPathItem::releaseExtents()
{
    emit minimumExtentsChanged(m_minimumExtents);
    emit maximumExtentsChanged(m_maximumExtents);
}

void QGLPathItem::processPreview(const pb::Preview &preview)
{
    switch (preview.type())
    {
    case pb::PV_STRAIGHT_PROBE:  /*nothing*/ return;
    case pb::PV_RIGID_TAP:  /*nothing*/ return;
    case pb::PV_STRAIGHT_FEED: processStraightMove(preview, FeedMove); return;
    case pb::PV_ARC_FEED: processArcFeed(preview); return;
    case pb::PV_STRAIGHT_TRAVERSE: processStraightMove(preview, TraverseMove); return;
    case pb::PV_SET_G5X_OFFSET: processSetG5xOffset(preview); return;
    case pb::PV_SET_G92_OFFSET: processSetG92Offset(preview); return;
    case pb::PV_SET_XY_ROTATION: /*nothing*/ return;
    case pb::PV_SELECT_PLANE: processSelectPlane(preview); return;
    case pb::PV_SET_TRAVERSE_RATE: /*nothing*/ return;
    case pb::PV_SET_FEED_RATE: /*nothing*/ return;
    case pb::PV_CHANGE_TOOL: /*nothing*/ return;
    case pb::PV_CHANGE_TOOL_NUMBER: /*nothing*/ return;
    case pb::PV_DWELL: /*nothing*/ return;
    case pb::PV_MESSAGE: /*nothing*/ return;
    case pb::PV_COMMENT: /*nothing*/ return;
    case pb::PV_USE_TOOL_OFFSET: processUseToolOffset(preview); return;
    case pb::PV_SET_PARAMS: /*nothing*/ return;
    case pb::PV_SET_FEED_MODE: /*nothing*/ return;
    case pb::PV_SOURCE_CONTEXT: /*nothing*/ return;
    }
}

void QGLPathItem::processStraightMove(const pb::Preview &preview, MovementType movementType)
{
#ifdef QT_DEBUG
    if (movementType == FeedMove)
    {
        qDebug() << "straight feed";
    }
    else
    {
        qDebug() << "straight traverse";
    }
#endif

    Position newPosition;
    QVector3D currentVector;
    QVector3D newVector;
    LinePathItem *linePathItem;

    linePathItem = new LinePathItem();
    newPosition = calculateNewPosition(preview.pos());
    currentVector = positionToVector3D(m_currentPosition);
    newVector = positionToVector3D(newPosition);

    linePathItem->position = currentVector;
    linePathItem->lineVector = newVector - currentVector;
    linePathItem->movementType = movementType;
    linePathItem->modelIndex = m_currentModelIndex,
    m_previewPathItems.append(linePathItem);
    m_modelPathMap.insert(m_currentModelIndex, linePathItem);   // mapping model index to the item

    m_currentPosition = newPosition;

    updateExtents(newVector);
}

void QGLPathItem::processArcFeed(const pb::Preview &preview)
{
#ifdef QT_DEBUG
    qDebug() << "arc feed";
#endif

    Position newPosition;
    QVector3D currentVector;
    QVector3D newVector;
    QVector2D startPoint;
    QVector2D endPoint;
    QVector2D centerPoint;
    QVector2D startVector;
    QVector2D endVector;
    double startAngle;
    double endAngle;
    double helixOffset;
    bool anticlockwise;
    double radius;
    ArcPathItem *arcPathItem;

    currentVector = positionToVector3D(m_currentPosition);
    newPosition = calculateNewPosition(preview.pos());

    if (m_activePlane == XYPlane)
    {
        arcPathItem = new ArcPathItem();
        newPosition.x = preview.first_end();
        newPosition.y = preview.second_end();
        newPosition.z = preview.axis_end_point();
        newVector = positionToVector3D(newPosition);

        startPoint.setX(currentVector.x());
        startPoint.setY(currentVector.y());

        helixOffset = newVector.z() - currentVector.z();
    }
    else if (m_activePlane == YZPlane)
    {
        arcPathItem = new ArcPathItem();
        newPosition.y = preview.first_end();
        newPosition.z = preview.second_end();
        newPosition.x = preview.axis_end_point();
        newVector = positionToVector3D(newPosition);

        startPoint.setX(currentVector.y());
        startPoint.setY(currentVector.z());

        helixOffset = newVector.x() - currentVector.x();
    }
    else if (m_activePlane == XZPlane)
    {
        arcPathItem = new ArcPathItem();
        newPosition.x = preview.first_end();
        newPosition.z = preview.second_end();
        newPosition.y = preview.axis_end_point();
        newVector = positionToVector3D(newPosition);

        startPoint.setX(currentVector.x());
        startPoint.setY(currentVector.z());

        helixOffset = newVector.y() - currentVector.y();
    }
    else
    {
        return; // not supported
    }

    endPoint.setX(preview.first_end());
    endPoint.setY(preview.second_end());
    centerPoint.setX(preview.first_axis());
    centerPoint.setY(preview.second_axis());
    startVector = startPoint - centerPoint;
    endVector = endPoint - centerPoint;

    startAngle = qAtan2(startVector.y(), startVector.x());
    if (startAngle < 0) {
        startAngle += 2 * M_PI;
    }
    endAngle = qAtan2(endVector.y(), endVector.x());
    if (endAngle < 0) {
        endAngle += 2 * M_PI;
    }
    anticlockwise = preview.rotation() >= 0;
    if (anticlockwise) {
        startAngle += 2.0 * M_PI * (qAbs((double)preview.rotation())-1.0);  // for rotation > 1 increase the endAngle
    }
    else {
        endAngle -= 2.0 * M_PI * (qAbs((double)preview.rotation())-1.0);  // for rotation > 1 decrease the startAngle
    }

    radius = centerPoint.distanceToPoint(startPoint);

    // calculate the extents of the arc
    double firstAngle;
    double secondAngle;
    // when viewed on the unit-circle
    bool point1 = false;    // phi=0        right
    bool point2 = false;    // phi=pi/2     top
    bool point3 = false;    // phi=pi       left
    bool point4 = false;    // phi=3pi/2    bottom

    if (anticlockwise) {
        firstAngle = endAngle;
        secondAngle = startAngle;
    }
    else {
        firstAngle = startAngle;
        secondAngle = endAngle;
    }

    if (secondAngle > firstAngle) {
        secondAngle = 2.0 * M_PI - secondAngle;
    }

    if ((firstAngle > 0.0) && (secondAngle < 0.0)) {
        point1 = true;
    }
    if (((firstAngle > M_PI_2) && (secondAngle < M_PI_2)) || (secondAngle < -3.0*M_PI_2)) {
        point2 = true;
    }
    if (((firstAngle > M_PI) && (secondAngle < M_PI)) || (secondAngle < -M_PI)) {
        point3 = true;
    }
    if (((firstAngle > 3.0*M_PI_2) && (secondAngle < 3.0*M_PI_2)) || (secondAngle < -M_PI_2)) {
        point4 = true;
    }

    updateExtents(newVector);
    if (m_activePlane == XYPlane)   // centerPoint: X is X, Y is Y
    {
        if (point1) {
            updateExtents(QVector3D(centerPoint.x() + radius,
                                    centerPoint.y(),
                                    currentVector.z()));
        }
        if (point2) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y() + radius,
                                    currentVector.z()));
        }
        if (point3) {
            updateExtents(QVector3D(centerPoint.x() - radius,
                                    centerPoint.y(),
                                    currentVector.z()));
        }
        if (point4) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y() - radius,
                                    currentVector.z()));
        }
    }
    else if (m_activePlane == XZPlane)  // centerPoint: X is X, Y is Z
    {
        if (point1) {
            updateExtents(QVector3D(centerPoint.x() + radius,
                                    currentVector.y(),
                                    centerPoint.y()));
        }
        if (point2) {
            updateExtents(QVector3D(centerPoint.x(),
                                    currentVector.y(),
                                    centerPoint.y() + radius));
        }
        if (point3) {
            updateExtents(QVector3D(centerPoint.x() - radius,
                                    currentVector.y(),
                                    centerPoint.y()));
        }
        if (point4) {
            updateExtents(QVector3D(centerPoint.x(),
                                    currentVector.y(),
                                    centerPoint.y() - radius));
        }
    }
    else if (m_activePlane == YZPlane)  // centerPoint: X is Y, Y is Z
    {
        if (point1) {
            updateExtents(QVector3D(currentVector.x(),
                                    centerPoint.x() + radius,
                                    centerPoint.y()));
        }
        if (point2) {
            updateExtents(QVector3D(currentVector.x(),
                                    centerPoint.x(),
                                    centerPoint.y() + radius));
        }
        if (point3) {
            updateExtents(QVector3D(currentVector.x(),
                                    centerPoint.x() - radius,
                                    centerPoint.y()));
        }
        if (point4) {
            updateExtents(QVector3D(currentVector.x(),
                                    centerPoint.x(),
                                    centerPoint.y() - radius));
        }
    }

    arcPathItem->position = currentVector;
    arcPathItem->rotationPlane = m_activePlane;
    arcPathItem->center = (centerPoint - startPoint);
    arcPathItem->radius = radius;
    arcPathItem->helixOffset = helixOffset;
    arcPathItem->startAngle = startAngle;
    arcPathItem->endAngle = endAngle;
    arcPathItem->anticlockwise = anticlockwise;
    arcPathItem->movementType = FeedMove;
    arcPathItem->modelIndex = m_currentModelIndex,
    m_previewPathItems.append(arcPathItem);
    m_modelPathMap.insertMulti(m_currentModelIndex, arcPathItem);   // mapping model index to the item

    m_currentPosition = newPosition;
}

void QGLPathItem::processSetG5xOffset(const pb::Preview &preview)
{
    if (preview.has_pos()) {
        m_activeOffsets.g5xOffsets.replace(preview.g5_index(), previewPositionToPosition(preview.pos()));
    }
}

void QGLPathItem::processSetG92Offset(const pb::Preview &preview)
{
    if (preview.has_pos()) {
        m_activeOffsets.g92Offset = previewPositionToPosition(preview.pos());
    }
}

void QGLPathItem::processUseToolOffset(const pb::Preview &preview)
{
    if (preview.has_pos()) {
        m_activeOffsets.toolOffset = previewPositionToPosition(preview.pos());
    }
}

void QGLPathItem::processSelectPlane(const pb::Preview &preview)
{
    if (preview.has_plane())
    {
        switch (preview.plane())
        {
        case 1: m_activePlane = XYPlane; break;
        case 2: m_activePlane = YZPlane; break;
        case 3: m_activePlane = XZPlane; break;
        case 4: m_activePlane = UVPlane; break;
        case 5: m_activePlane = VWPlane; break;
        case 6: m_activePlane = WUPlane; break;
        default: break;
        }
    }
}

QGLPathItem::Position QGLPathItem::previewPositionToPosition(const pb::Position &position) const
{
    Position newPosition;
    newPosition.x = 0.0;
    newPosition.y = 0.0;
    newPosition.z = 0.0;
    newPosition.a = 0.0;
    newPosition.b = 0.0;
    newPosition.c = 0.0;
    newPosition.u = 0.0;
    newPosition.v = 0.0;
    newPosition.w = 0.0;

    if (position.has_x()) {
        newPosition.x = position.x();
    }
    if (position.has_y()) {
        newPosition.y = position.y();
    }
    if (position.has_z()) {
        newPosition.z = position.z();
    }
    if (position.has_a()) {
        newPosition.a = position.a();
    }
    if (position.has_b()) {
        newPosition.b = position.b();
    }
    if (position.has_c()) {
        newPosition.c = position.c();
    }
    if (position.has_u()) {
        newPosition.u = position.u();
    }
    if (position.has_v()) {
        newPosition.v = position.v();
    }
    if (position.has_w()) {
        newPosition.w = position.w();
    }

    return newPosition;
}

QGLPathItem::Position QGLPathItem::calculateNewPosition(const pb::Position &newPosition) const
{
    Position position = m_currentPosition;

    if (newPosition.has_x()) {
        position.x = m_activeOffsets.g92Offset.x;
        position.x += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).x;
        position.x += m_activeOffsets.toolOffset.x;
        position.x += newPosition.x();
    }

    if (newPosition.has_y()) {
        position.y = m_activeOffsets.g92Offset.y;
        position.y += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).y;
        position.y += m_activeOffsets.toolOffset.y;
        position.y += newPosition.y();
    }

    if (newPosition.has_z()) {
        position.z = m_activeOffsets.g92Offset.z;
        position.z += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).z;
        position.z += m_activeOffsets.toolOffset.z;
        position.z += newPosition.z();
    }

    if (newPosition.has_a()) {
        position.a = m_activeOffsets.g92Offset.a;
        position.a += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).a;
        position.a += m_activeOffsets.toolOffset.a;
        position.a += newPosition.a();
    }
    if (newPosition.has_b()) {
        position.b = m_activeOffsets.g92Offset.b;
        position.b += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).b;
        position.b += m_activeOffsets.toolOffset.b;
        position.b += newPosition.b();
    }
    if (newPosition.has_c()) {
        position.c = m_activeOffsets.g92Offset.c;
        position.c += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).c;
        position.c += m_activeOffsets.toolOffset.c;
        position.c += newPosition.c();
    }
    if (newPosition.has_u()) {
        position.u = m_activeOffsets.g92Offset.u;
        position.u += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).u;
        position.u += m_activeOffsets.toolOffset.u;
        position.u += newPosition.u();
    }
    if (newPosition.has_v()) {
        position.v = m_activeOffsets.g92Offset.v;
        position.v += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).v;
        position.v += m_activeOffsets.toolOffset.v;
        position.v += newPosition.v();
    }
    if (newPosition.has_w()) {
        position.w = m_activeOffsets.g92Offset.w;
        position.w += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).w;
        position.w += m_activeOffsets.toolOffset.w;
        position.w += newPosition.w();
    }

    return position;
}

QVector3D QGLPathItem::positionToVector3D(const QGLPathItem::Position &position) const
{
    return QVector3D(position.x, position.y, position.z);
}

void QGLPathItem::drawPath()
{

    if (m_model == nullptr)
    {
        return;
    }

    qDeleteAll(m_previewPathItems); // clear the list of preview path items
    m_previewPathItems.clear();
    resetActiveOffsets(); // clear the offsets
    resetActivePlane();
    resetCurrentPosition();  // reset current position
    resetRelativePosition(); // reset relative position
    resetExtents();

    m_modelPathMap.clear();
    m_drawablePathMap.clear();
    m_previousSelectedDrawable = nullptr;

    QLinkedList<QGCodeProgramModel::PreviewItem> previewItems = m_model->previewItems();
    QLinkedListIterator<QGCodeProgramModel::PreviewItem> i(previewItems);
    while (i.hasNext())
    {
        QGCodeProgramModel::PreviewItem item = i.next();
        m_currentModelIndex = item.modelIndex;
        processPreview(item.previewItem);
    }

    m_needsFullUpdate = true;
    emit needsUpdate();

    releaseExtents();
}

void QGLPathItem::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(bottomRight) // we only change one item at a time
    if (roles.contains(QGCodeProgramModel::SelectedRole)
        || roles.contains(QGCodeProgramModel::ActiveRole)
        || roles.contains(QGCodeProgramModel::ExecutedRole))
    {
        QList<PathItem*> pathItemList;

        pathItemList = m_modelPathMap.values(topLeft);
        if (!pathItemList.isEmpty())
        {
            m_modifiedPathItems.append(pathItemList);
            emit needsUpdate();
        }
    }
}

void QGLPathItem::triggerFullUpdate()
{
    m_needsFullUpdate = true;
}
