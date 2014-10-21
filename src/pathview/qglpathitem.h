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

#ifndef QGLPATHITEM_H
#define QGLPATHITEM_H

#include "qglitem.h"
#include "qgcodeprogrammodel.h"
#include "preview.pb.h"

class QGLPathItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QColor arcFeedColor READ arcFeedColor WRITE setArcFeedColor NOTIFY arcFeedColorChanged)
    Q_PROPERTY(QColor straightFeedColor READ straightFeedColor WRITE setStraightFeedColor NOTIFY straightFeedColorChanged)
    Q_PROPERTY(QColor traverseColor READ traverseColor WRITE setTraverseColor NOTIFY traverseColorChanged)
    Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QVector3D minimumExtents READ minimumExtents NOTIFY minimumExtentsChanged)
    Q_PROPERTY(QVector3D maximumExtents READ maximumExtents NOTIFY maximumExtentsChanged)

public:
    explicit QGLPathItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QGCodeProgramModel * model() const;
    QColor arcFeedColor() const;
    QColor traverseColor() const;
    QColor selectedColor() const;
    QVector3D minimumExtents() const;
    QVector3D maximumExtents() const;
    QColor straightFeedColor() const;

public slots:
    virtual void selectDrawable(void *pointer);

    void setModel(QGCodeProgramModel * arg);
    void setArcFeedColor(QColor arg);
    void setTraverseColor(QColor arg);
    void setSelectedColor(QColor arg);
    void setStraightFeedColor(QColor arg);

private:
    struct Position {
        double x;
        double y;
        double z;
        double a;
        double b;
        double c;
        double u;
        double v;
        double w;
    };

    struct Offsets {
        Position g92Offset;
        Position toolOffset;
        QList<Position> g5xOffsets;
        int g5xOffsetIndex;
    };

    enum PathType {
        Line,
        Arc
    };

    enum MovementType {
        FeedMove,
        TraverseMove
    };

    enum Plane {
        XYPlane,
        XZPlane,
        YZPlane,
        UVPlane,
        WUPlane,
        VWPlane
    };

    class PathItem {
    public:
        PathItem():
            pathType(Line),
            movementType(FeedMove),
            drawablePointer(NULL){}

        PathType pathType;
        MovementType movementType;
        QVector3D position;
        QModelIndex modelIndex;
        void *drawablePointer;
    };

    class LinePathItem: public PathItem {
    public:
        LinePathItem(): PathItem() {
            pathType = Line;
        }

        QVector3D lineVector;
    };

    class ArcPathItem: public PathItem {
    public:
        ArcPathItem():
            PathItem(),
            helixOffset(0.0),
            radius(0.0),
            startAngle(0.0),
            endAngle(0.0),
            anticlockwise(false),
            rotationPlane(XYPlane)
        {
            pathType = Arc;
        }

        QVector2D center;
        double helixOffset;     // offset from the rotation axis
        double radius;
        double startAngle;
        double endAngle;
        bool anticlockwise;
        Plane rotationPlane;
    };

    QGCodeProgramModel * m_model;
    QColor m_arcFeedColor;
    QColor m_straightFeedColor;
    QColor m_traverseColor;
    QColor m_selectedColor;

    Offsets m_activeOffsets;
    Position m_currentPosition;
    Plane m_activePlane;
    QList<PathItem*> m_previewPathItems;
    QModelIndex m_currentModelIndex;
    QMultiMap<QModelIndex, PathItem*> m_modelPathMap;  // for mapping the model to internal items
    QMap<void*, PathItem*> m_drawablePathMap;  // for mapping GL views drawables to internal items
    void* m_previousSelectedDrawable;

    bool m_needsFullUpdate;
    QList<PathItem*> m_modifiedPathItems;

    QVector3D m_minimumExtents;
    QVector3D m_maximumExtents;

    void resetActiveOffsets();
    void resetCurrentPosition();
    void resetActivePlane();
    void resetExtents();
    void updateExtents(const QVector3D &vector);
    void releaseExtents();
    void processPreview(const pb::Preview &preview);
    void processStraightMove(const pb::Preview &preview, MovementType movementType);
    void processArcFeed(const pb::Preview &preview);
    void processSetG5xOffset(const pb::Preview &preview);
    void processSetG92Offset(const pb::Preview &preview);
    void processUseToolOffset(const pb::Preview &preview);
    void processSelectPlane(const pb::Preview &preview);
    Position previewPositionToPosition(const pb::Position &position) const;
    Position calculateNewPosition(const pb::Position &newPosition) const;
    QVector3D positionToVector3D(const Position &position) const;

private slots:
    void drawPath();
    void modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles);
    void triggerFullUpdate();

signals:
    void modelChanged(QGCodeProgramModel * arg);
    void arcFeedColorChanged(QColor arg);
    void traverseColorChanged(QColor arg);
    void selectedColorChanged(QColor arg);
    void minimumExtentsChanged(QVector3D arg);
    void maximumExtentsChanged(QVector3D arg);
    void straightFeedColorChanged(QColor arg);
};

#endif // QGLPATHITEM_H
