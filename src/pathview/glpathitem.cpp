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

#include "glpathitem.h"
#include <QtCore/qmath.h>
#include <cmath>
#include "debughelper.h"

const float PI_F = 3.14159265358979f;
const float PI_2_F = 1.57079632679489f;

using namespace machinetalk;

namespace qtquickvcp {

GLPathItem::GLPathItem(QQuickItem *parent)
    : GLItem(parent)
    , m_model(nullptr)
    , m_arcFeedColor(QColor(Qt::white))
    , m_straightFeedColor(QColor(Qt::white))
    , m_traverseColor(QColor(Qt::cyan))
    , m_backplotArcFeedColor(QColor(Qt::yellow))
    , m_backplotStraightFeedColor(QColor(Qt::yellow))
    , m_backplotTraverseColor(QColor(Qt::yellow))
    , m_selectedColor(QColor(Qt::magenta))
    , m_activeColor(QColor(Qt::red))
    , m_lineWidth(1.0)
    , m_traverseLineStippleLength(1.0)
    , m_arcDivision(16)
    , m_activePlane(XYPlane)
    , m_needsFullUpdate(true)
    , m_minimumExtents(QVector3D(0, 0, 0))
    , m_maximumExtents(QVector3D(0, 0, 0))
    , m_extentsUpdated(false)
{
    connect(this, &GLPathItem::visibleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::positionChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::scaleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::rotationChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::rotationAngleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::rotationAxisChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::visibleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::lineWidthChanged,
            this, &GLPathItem::triggerFullUpdate);
}

GLPathItem::~GLPathItem()
{
    qDeleteAll(m_previewPathItems);
}

void GLPathItem::paint(GLView *glView)
{
    if (m_needsFullUpdate)
    {
        glView->prepare(this);
        glView->reset();
        glView->lineWidth(m_lineWidth);
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
                    glView->rotate(-90, 0, 1, 0);
                    glView->rotate(-90, 1, 0, 0);
                }
                else if  (arcPathItem->rotationPlane == YZPlane) {
                    glView->rotate(90, 1, 0, 0);
                    glView->rotate(90, 0, 1, 0);
                }
                drawablePointer = glView->arc(arcPathItem->center.x(),
                                              arcPathItem->center.y(),
                                              arcPathItem->radius,
                                              arcPathItem->startAngle,
                                              arcPathItem->endAngle,
                                              arcPathItem->anticlockwise,
                                              arcPathItem->helixOffset,
                                              m_arcDivision);
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
        for (PathItem *pathItem: qAsConst(m_modifiedPathItems))
        {
            if (pathItem != nullptr)
            {
                QColor color;
                if (m_model->data(pathItem->modelIndex, GCodeProgramModel::SelectedRole).toBool()) {
                    color = m_selectedColor;
                }
                else if (m_model->data(pathItem->modelIndex, GCodeProgramModel::ActiveRole).toBool())
                {
                    color = m_activeColor;
                }
                else if (m_model->data(pathItem->modelIndex, GCodeProgramModel::ExecutedRole).toBool())
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

GCodeProgramModel *GLPathItem::model() const
{
    return m_model;
}

QColor GLPathItem::arcFeedColor() const
{
    return m_arcFeedColor;
}

QColor GLPathItem::traverseColor() const
{
    return m_traverseColor;
}

QColor GLPathItem::selectedColor() const
{
    return m_selectedColor;
}

QVector3D GLPathItem::minimumExtents() const
{
    return m_minimumExtents;
}

QVector3D GLPathItem::maximumExtents() const
{
    return m_maximumExtents;
}

float GLPathItem::lineWidth() const
{
    return m_lineWidth;
}

float GLPathItem::traverseLineStippleLength() const
{
    return m_traverseLineStippleLength;
}

int GLPathItem::arcDivision() const
{
    return m_arcDivision;
}

QColor GLPathItem::straightFeedColor() const
{
    return m_straightFeedColor;
}

QColor GLPathItem::activeColor() const
{
    return m_activeColor;
}

QColor GLPathItem::backplotArcFeedColor() const
{
    return m_backplotArcFeedColor;
}

QColor GLPathItem::backplotStraightFeedColor() const
{
    return m_backplotStraightFeedColor;
}

QColor GLPathItem::backplotTraverseColor() const
{
    return m_backplotTraverseColor;
}

void GLPathItem::selectDrawable(void *pointer)
{
    if (m_model == nullptr) {
        return;
    }

    PathItem *mappedPathItem = m_drawablePathMap.value(pointer, nullptr);
    if (mappedPathItem == nullptr) {
        return;
    }

    const QModelIndex &mappedModelIndex = mappedPathItem->modelIndex;
    m_model->clearSelectionAndSelectLine(mappedModelIndex);
    m_modifiedPathItems.append(mappedPathItem);
    emit needsUpdate();
}

void GLPathItem::setModel(GCodeProgramModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged(arg);

        if (m_model != nullptr)
        {
            connect(m_model, &QAbstractItemModel::modelReset,
                    this, &GLPathItem::drawPath);
            connect(m_model, &QAbstractItemModel::dataChanged,
                    this, &GLPathItem::modelDataChanged);

            if (m_model->rowCount() > 0)
            {
                drawPath();     // draw model when set
            }
        }
    }
}

void GLPathItem::setArcFeedColor(QColor arg)
{
    if (m_arcFeedColor != arg) {
        m_arcFeedColor = arg;
        emit arcFeedColorChanged(arg);
    }
}

void GLPathItem::setTraverseColor(QColor arg)
{
    if (m_traverseColor != arg) {
        m_traverseColor = arg;
        emit traverseColorChanged(arg);
    }
}

void GLPathItem::setSelectedColor(QColor arg)
{
    if (m_selectedColor != arg) {
        m_selectedColor = arg;
        emit selectedColorChanged(arg);
    }
}

void GLPathItem::setStraightFeedColor(QColor arg)
{
    if (m_straightFeedColor != arg) {
        m_straightFeedColor = arg;
        emit straightFeedColorChanged(arg);
    }
}

void GLPathItem::setActiveColor(QColor arg)
{
    if (m_activeColor != arg) {
        m_activeColor = arg;
        emit activeColorChanged(arg);
    }
}

void GLPathItem::setLineWidth(float arg)
{
    if (m_lineWidth != arg) {
        m_lineWidth = arg;
        emit lineWidthChanged(arg);
    }
}

void GLPathItem::setTraverseLineStippleLength(float arg)
{
    if (m_traverseLineStippleLength != arg) {
        m_traverseLineStippleLength = arg;
        emit traverseLineStippleLengthChanged(arg);
    }
}

void GLPathItem::setArcDivision(int arcDivision)
{
    if (m_arcDivision == arcDivision) {
        return;
    }

    m_arcDivision = arcDivision;
    emit arcDivisionChanged(arcDivision);
}

void GLPathItem::setBackplotArcFeedColor(QColor arg)
{
    if (m_backplotArcFeedColor != arg) {
        m_backplotArcFeedColor = arg;
        emit backplotArcFeedColorChanged(arg);
    }
}

void GLPathItem::setBackplotStraightFeedColor(QColor arg)
{
    if (m_backplotStraightFeedColor != arg) {
        m_backplotStraightFeedColor = arg;
        emit backplotStraightFeedColorChanged(arg);
    }
}

void GLPathItem::setBackplotTraverseColor(QColor arg)
{
    if (m_backplotTraverseColor != arg) {
        m_backplotTraverseColor = arg;
        emit backplotTraverseColorChanged(arg);
    }
}

void GLPathItem::resetActiveOffsets()
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

void GLPathItem::resetCurrentPosition()
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

void GLPathItem::resetRelativePosition()
{
    m_relativePosition.set_x(0.0);
    m_relativePosition.set_y(0.0);
    m_relativePosition.set_z(0.0);
    m_relativePosition.set_a(0.0);
    m_relativePosition.set_b(0.0);
    m_relativePosition.set_c(0.0);
    m_relativePosition.set_u(0.0);
    m_relativePosition.set_v(0.0);
    m_relativePosition.set_w(0.0);
}

void GLPathItem::resetActivePlane()
{
    m_activePlane = XYPlane;
}

void GLPathItem::resetExtents()
{
    m_maximumExtents = QVector3D();
    m_minimumExtents = QVector3D();
    m_extentsUpdated = false;
}

void GLPathItem::updateExtents(const QVector3D &vector)
{
    if ((vector.x() < m_minimumExtents.x()) || !m_extentsUpdated) {
        m_minimumExtents.setX(vector.x());
    }
    if ((vector.y() < m_minimumExtents.y()) || !m_extentsUpdated) {
        m_minimumExtents.setY(vector.y());
    }
    if ((vector.z() < m_minimumExtents.z()) || !m_extentsUpdated) {
        m_minimumExtents.setZ(vector.z());
    }
    if ((vector.x() > m_maximumExtents.x()) || !m_extentsUpdated) {
        m_maximumExtents.setX(vector.x());
    }
    if ((vector.y() > m_maximumExtents.y()) || !m_extentsUpdated) {
        m_maximumExtents.setY(vector.y());
    }
    if ((vector.z() > m_maximumExtents.z()) || !m_extentsUpdated) {
        m_maximumExtents.setZ(vector.z());
    }
    m_extentsUpdated = true;
}

void GLPathItem::releaseExtents()
{
    emit minimumExtentsChanged(m_minimumExtents);
    emit maximumExtentsChanged(m_maximumExtents);
}

void GLPathItem::processPreview(const Preview &preview)
{
    switch (preview.type())
    {
    case PV_STRAIGHT_PROBE:  /*nothing*/ return;
    case PV_RIGID_TAP:  /*nothing*/ return;
    case PV_STRAIGHT_FEED: processStraightMove(preview, FeedMove); return;
    case PV_ARC_FEED: processArcFeed(preview); return;
    case PV_STRAIGHT_TRAVERSE: processStraightMove(preview, TraverseMove); return;
    case PV_SET_G5X_OFFSET: processSetG5xOffset(preview); return;
    case PV_SET_G92_OFFSET: processSetG92Offset(preview); return;
    case PV_SET_XY_ROTATION: /*nothing*/ return;
    case PV_SELECT_PLANE: processSelectPlane(preview); return;
    case PV_SET_TRAVERSE_RATE: /*nothing*/ return;
    case PV_SET_FEED_RATE: /*nothing*/ return;
    case PV_CHANGE_TOOL: /*nothing*/ return;
    case PV_CHANGE_TOOL_NUMBER: /*nothing*/ return;
    case PV_DWELL: /*nothing*/ return;
    case PV_MESSAGE: /*nothing*/ return;
    case PV_COMMENT: /*nothing*/ return;
    case PV_USE_TOOL_OFFSET: processUseToolOffset(preview); return;
    case PV_SET_PARAMS: /*nothing*/ return;
    case PV_SET_FEED_MODE: /*nothing*/ return;
    case PV_SOURCE_CONTEXT: /*nothing*/ return;
    case PV_PREVIEW_START: /*nothing*/ return;
    case PV_PREVIEW_END: /*nothing*/ return;
    }
}

void GLPathItem::processStraightMove(const Preview &preview, MovementType movementType)
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
    currentVector = positionToVector3D(m_currentPosition);
    newPosition = calculateNewPosition(preview.pos());
    newVector = positionToVector3D(newPosition);

    linePathItem->position = currentVector;
    linePathItem->lineVector = newVector - currentVector;
    linePathItem->movementType = movementType;
    linePathItem->modelIndex = m_currentModelIndex;
    m_previewPathItems.append(linePathItem);
    m_modelPathMap.insert(m_currentModelIndex, linePathItem);   // mapping model index to the item

    m_currentPosition = newPosition;
    m_relativePosition = preview.pos();

    updateExtents(newVector);
}

void GLPathItem::processArcFeed(const Preview &preview)
{
#ifdef QT_DEBUG
    qDebug() << "arc feed";
#endif

    // the index values are used for mapping the plane dependant preview data to cartesian space
    int iX;
    int iY;
    int iZ;
    if (m_activePlane == XYPlane)
    {
        iX = 0;
        iY = 1;
        iZ = 2;
    }
    else if (m_activePlane == YZPlane)
    {
        iX = 1;
        iY = 2;
        iZ = 0;
    }
    else if (m_activePlane == XZPlane)
    {
        iX = 2;
        iY = 0;
        iZ = 1;
        // Note: it looks like the X and Z axes come inverted from preview
    }
    else
    {
        return; // not supported
    }

    const QVector3D startPoint = positionToVector3D(m_currentPosition);
    const Position newPosition = calculateNewPosition(preview.pos());
    const QVector3D endPoint = positionToVector3D(newPosition);
    const float helixOffset = endPoint[iZ] - startPoint[iZ];

    QVector3D offset;
    offset[0] = static_cast<float>(preview.pos().x()) - newPosition.x;
    offset[1] = static_cast<float>(preview.pos().y()) - newPosition.y;
    offset[2] = static_cast<float>(preview.pos().z()) - newPosition.z;
    const float cx = static_cast<float>(preview.first_axis()) + offset[iX];
    const float cy = static_cast<float>(preview.second_axis()) + offset[iY];
    QVector3D centerPoint;
    centerPoint[iZ] = startPoint[iZ];
    centerPoint[iX] = cx;
    centerPoint[iY] = cy;

    float startAngle = std::atan2(startPoint[iY] - cy, startPoint[iX] - cx);
    float endAngle = std::atan2(endPoint[iY] - cy, endPoint[iX] - cx);
    const bool anticlockwise = preview.rotation() >= 0;
    if (anticlockwise) {
        startAngle += 2.0f * PI_F * (std::fabs(static_cast<float>(preview.rotation())) - 1.0f);  // for rotation > 1 increase the endAngle
    }
    else {
        endAngle -= 2.0f * PI_F * (std::fabs(static_cast<float>(preview.rotation())) - 1.0f);  // for rotation > 1 decrease the startAngle
    }

    const float radius = centerPoint.distanceToPoint(startPoint);

    // calculate the extents of the arc
    // when viewed on the unit-circle
    bool point1 = false;    // phi=0        right
    bool point2 = false;    // phi=pi/2     top
    bool point3 = false;    // phi=pi       left
    bool point4 = false;    // phi=3pi/2    bottom

    if ((startAngle > 0.0f) && (endAngle < 0.0f)) {
        point1 = true;
    }
    if (((startAngle > PI_2_F) && (endAngle < PI_2_F)) || (endAngle < -3.0f * PI_2_F)) {
        point2 = true;
    }
    if (((startAngle > PI_F) && (endAngle < PI_F)) || (endAngle < -PI_F)) {
        point3 = true;
    }
    if (((startAngle > 3.0f * PI_2_F) && (endAngle < 3.0f * PI_2_F)) || (endAngle < -PI_2_F)) {
        point4 = true;
    }

    updateExtents(startPoint);
    if (m_activePlane == XYPlane)   // centerPoint: X is X, Y is Y
    {
        if (point1) {
            updateExtents(QVector3D(centerPoint.x() + radius,
                                    centerPoint.y(),
                                    centerPoint.z()));
        }
        if (point2) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y() + radius,
                                    centerPoint.z()));
        }
        if (point3) {
            updateExtents(QVector3D(centerPoint.x() - radius,
                                    centerPoint.y(),
                                    centerPoint.z()));
        }
        if (point4) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y() - radius,
                                    centerPoint.z()));
        }
    }
    else if (m_activePlane == XZPlane)  // centerPoint: X is X, Y is Z
    {
        if (point1) {
            updateExtents(QVector3D(centerPoint.x() + radius,
                                    centerPoint.y(),
                                    centerPoint.z()));
        }
        if (point2) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y(),
                                    centerPoint.z() + radius));
        }
        if (point3) {
            updateExtents(QVector3D(centerPoint.x() - radius,
                                    centerPoint.y(),
                                    centerPoint.z()));
        }
        if (point4) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y(),
                                    centerPoint.z() - radius));
        }
    }
    else if (m_activePlane == YZPlane)  // centerPoint: X is Y, Y is Z
    {
        if (point1) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y() + radius,
                                    centerPoint.z()));
        }
        if (point2) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y(),
                                    centerPoint.z() + radius));
        }
        if (point3) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y() - radius,
                                    centerPoint.z()));
        }
        if (point4) {
            updateExtents(QVector3D(centerPoint.x(),
                                    centerPoint.y(),
                                    centerPoint.z() - radius));
        }
    }

    auto arcPathItem = new ArcPathItem();
    arcPathItem->position = startPoint;
    arcPathItem->rotationPlane = m_activePlane;
    arcPathItem->center = QVector2D(centerPoint[iX] - startPoint[iX], centerPoint[iY] - startPoint[iY]);
    arcPathItem->radius = radius;
    arcPathItem->helixOffset = helixOffset;
    arcPathItem->startAngle = startAngle;
    arcPathItem->endAngle = endAngle;
    arcPathItem->anticlockwise = anticlockwise;
    arcPathItem->movementType = FeedMove;
    arcPathItem->modelIndex = m_currentModelIndex;
    m_previewPathItems.append(arcPathItem);
    m_modelPathMap.insertMulti(m_currentModelIndex, arcPathItem);   // mapping model index to the item

    m_currentPosition = newPosition;
    m_relativePosition = preview.pos();
}

void GLPathItem::processSetG5xOffset(const Preview &preview)
{
    if (preview.has_pos() && preview.has_g5_index()) {
        m_activeOffsets.g5xOffsetIndex = preview.g5_index();
        m_activeOffsets.g5xOffsets.replace(preview.g5_index()-1, previewPositionToPosition(preview.pos()));
        m_currentPosition = calculateNewPosition(m_relativePosition);
    }
}

void GLPathItem::processSetG92Offset(const Preview &preview)
{
    if (preview.has_pos()) {
        m_activeOffsets.g92Offset = previewPositionToPosition(preview.pos());
        m_currentPosition = calculateNewPosition(m_relativePosition);
    }
}

void GLPathItem::processUseToolOffset(const Preview &preview)
{
    if (preview.has_pos()) {
        m_activeOffsets.toolOffset = previewPositionToPosition(preview.pos());
        m_currentPosition = calculateNewPosition(m_relativePosition);
    }
}

void GLPathItem::processSelectPlane(const Preview &preview)
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

GLPathItem::Position GLPathItem::previewPositionToPosition(const machinetalk::Position &position) const
{
    Position newPosition;
    newPosition.x = 0.0f;
    newPosition.y = 0.0f;
    newPosition.z = 0.0f;
    newPosition.a = 0.0f;
    newPosition.b = 0.0f;
    newPosition.c = 0.0f;
    newPosition.u = 0.0f;
    newPosition.v = 0.0f;
    newPosition.w = 0.0f;

    if (position.has_x()) {
        newPosition.x = static_cast<float>(position.x());
    }
    if (position.has_y()) {
        newPosition.y = static_cast<float>(position.y());
    }
    if (position.has_z()) {
        newPosition.z = static_cast<float>(position.z());
    }
    if (position.has_a()) {
        newPosition.a = static_cast<float>(position.a());
    }
    if (position.has_b()) {
        newPosition.b = static_cast<float>(position.b());
    }
    if (position.has_c()) {
        newPosition.c = static_cast<float>(position.c());
    }
    if (position.has_u()) {
        newPosition.u = static_cast<float>(position.u());
    }
    if (position.has_v()) {
        newPosition.v = static_cast<float>(position.v());
    }
    if (position.has_w()) {
        newPosition.w = static_cast<float>(position.w());
    }

    return newPosition;
}

GLPathItem::Position GLPathItem::calculateNewPosition(const machinetalk::Position &newPosition) const
{
    Position position = m_currentPosition;

    if (newPosition.has_x()) {
        position.x = m_activeOffsets.g92Offset.x;
        position.x += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).x;
        position.x += newPosition.x();
    }

    if (newPosition.has_y()) {
        position.y = m_activeOffsets.g92Offset.y;
        position.y += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).y;
        position.y += newPosition.y();
    }

    if (newPosition.has_z()) {
        position.z = m_activeOffsets.g92Offset.z;
        position.z += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).z;
        position.z += newPosition.z();
    }

    if (newPosition.has_a()) {
        position.a = m_activeOffsets.g92Offset.a;
        position.a += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).a;
        position.a += newPosition.a();
    }
    if (newPosition.has_b()) {
        position.b = m_activeOffsets.g92Offset.b;
        position.b += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).b;
        position.b += newPosition.b();
    }
    if (newPosition.has_c()) {
        position.c = m_activeOffsets.g92Offset.c;
        position.c += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).c;
        position.c += newPosition.c();
    }
    if (newPosition.has_u()) {
        position.u = m_activeOffsets.g92Offset.u;
        position.u += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).u;
        position.u += newPosition.u();
    }
    if (newPosition.has_v()) {
        position.v = m_activeOffsets.g92Offset.v;
        position.v += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).v;
        position.v += newPosition.v();
    }
    if (newPosition.has_w()) {
        position.w = m_activeOffsets.g92Offset.w;
        position.w += m_activeOffsets.g5xOffsets.at(m_activeOffsets.g5xOffsetIndex-1).w;
        position.w += newPosition.w();
    }

    return position;
}

QVector3D GLPathItem::positionToVector3D(const GLPathItem::Position &position) const
{
    return QVector3D(position.x, position.y, position.z);
}

void GLPathItem::drawPath()
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

    QLinkedList<GCodeProgramModel::PreviewItem> previewItems = m_model->previewItems();
    QLinkedListIterator<GCodeProgramModel::PreviewItem> i(previewItems);
    while (i.hasNext())
    {
        GCodeProgramModel::PreviewItem item = i.next();
        m_currentModelIndex = item.modelIndex;
        processPreview(item.previewItem);
    }

    m_needsFullUpdate = true;
    emit needsUpdate();

    releaseExtents();
}

void GLPathItem::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(bottomRight) // we only change one item at a time
    if (roles.contains(GCodeProgramModel::SelectedRole)
        || roles.contains(GCodeProgramModel::ActiveRole)
        || roles.contains(GCodeProgramModel::ExecutedRole))
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

void GLPathItem::triggerFullUpdate()
{
    m_needsFullUpdate = true;
    emit needsUpdate();
}
} // namespace qtquickvcp
