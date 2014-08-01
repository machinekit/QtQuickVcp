#include "qglpathitem.h"
#include <QtCore/qmath.h>
#include "debughelper.h"

QGLPathItem::QGLPathItem(QQuickItem *parent) :
    QGLItem(parent),
    m_model(NULL),
    m_feedColor(QColor(Qt::white)),
    m_traverseColor(QColor(Qt::cyan))
{
}

void QGLPathItem::paint(QGLView *glView)
{
    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->reset();
    glView->color(QColor(Qt::white));
    glView->beginUnion();
    for (int i = 0; i < m_previewPathItems.size(); ++i)
    {
        PathItem *pathItem = m_previewPathItems.at(i);
        if (pathItem->pathType == Line)
        {
            LinePathItem *linePathItem = static_cast<LinePathItem*>(pathItem);
            if (linePathItem->movementType == FeedMove)
            {
                glView->color(m_feedColor);
            }
            else
            {
                glView->color(m_traverseColor);
                glView->lineStipple(true, 1.0);
            }
            glView->translate(linePathItem->position);
            glView->line(linePathItem->lineVector);
        }
        else if (pathItem->pathType == Arc)
        {

        }
    }
    glView->endUnion();
}

QGCodeProgramModel *QGLPathItem::model() const
{
    return m_model;
}

QColor QGLPathItem::feedColor() const
{
    return m_feedColor;
}

QColor QGLPathItem::traverseColor() const
{
    return m_traverseColor;
}

void QGLPathItem::selectDrawable(void *pointer)
{

}

void QGLPathItem::setModel(QGCodeProgramModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged(arg);

        connect(m_model, SIGNAL(modelReset()),
                this, SLOT(drawPath()));
    }
}

void QGLPathItem::setFeedColor(QColor arg)
{
    if (m_feedColor != arg) {
        m_feedColor = arg;
        emit feedColorChanged(arg);
    }
}

void QGLPathItem::setTraverseColor(QColor arg)
{
    if (m_traverseColor != arg) {
        m_traverseColor = arg;
        emit traverseColorChanged(arg);
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
    case pb::PV_SELECT_PLANE: /*nothing*/ return;
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
    }
}

void QGLPathItem::processStraightMove(const pb::Preview &preview, MovementType movementType)
{
    if (movementType == FeedMove)
    {
        qDebug() << "straight feed";
    }
    else
    {
        qDebug() << "straight traverse";
    }

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
    m_previewPathItems.append(linePathItem);

    m_currentPosition = newPosition;
}

void QGLPathItem::processArcFeed(const pb::Preview &preview)
{
    qDebug() << "arc feed";
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

    if (m_model == NULL)
    {
        return;
    }

    qDeleteAll(m_previewPathItems); // clear the list of preview path items
    resetActiveOffsets(); // clear the offsets
    resetCurrentPosition(); // reset position

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        QModelIndex index;
        QList<pb::Preview>* previewList;

        index = m_model->index(i);
        previewList = static_cast<QList<pb::Preview>* >(m_model->data(index, QGCodeProgramModel::PreviewRole).value<void*>());

        if (previewList != NULL)
        {
            for (int j = 0; j < previewList->size(); ++j)
            {
                processPreview(previewList->at(j));
            }
        }
    }

    emit needsUpdate();
}
