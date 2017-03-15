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

#ifndef GLPATHITEM_H
#define GLPATHITEM_H

#include "glitem.h"
#include "gcodeprogrammodel.h"
#include <machinetalk/protobuf/preview.pb.h>

namespace qtquickvcp {

class GLPathItem : public GLItem
{
    Q_OBJECT
    Q_PROPERTY(QColor arcFeedColor READ arcFeedColor WRITE setArcFeedColor NOTIFY arcFeedColorChanged)
    Q_PROPERTY(QColor straightFeedColor READ straightFeedColor WRITE setStraightFeedColor NOTIFY straightFeedColorChanged)
    Q_PROPERTY(QColor traverseColor READ traverseColor WRITE setTraverseColor NOTIFY traverseColorChanged)
    Q_PROPERTY(QColor backplotArcFeedColor READ backplotArcFeedColor WRITE setBackplotArcFeedColor NOTIFY backplotArcFeedColorChanged)
    Q_PROPERTY(QColor backplotStraightFeedColor READ backplotStraightFeedColor WRITE setBackplotStraightFeedColor NOTIFY backplotStraightFeedColorChanged)
    Q_PROPERTY(QColor backplotTraverseColor READ backplotTraverseColor WRITE setBackplotTraverseColor NOTIFY backplotTraverseColorChanged)
    Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QColor activeColor READ activeColor WRITE setActiveColor NOTIFY activeColorChanged)
    Q_PROPERTY(GCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QVector3D minimumExtents READ minimumExtents NOTIFY minimumExtentsChanged)
    Q_PROPERTY(QVector3D maximumExtents READ maximumExtents NOTIFY maximumExtentsChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(float traverseLineStippleLength READ traverseLineStippleLength WRITE setTraverseLineStippleLength NOTIFY traverseLineStippleLengthChanged)
    Q_PROPERTY(int arcDivision READ arcDivision WRITE setArcDivision NOTIFY arcDivisionChanged)

public:
    explicit GLPathItem(QQuickItem *parent = 0);
    ~GLPathItem();

    virtual void paint(GLView *glView);

    GCodeProgramModel * model() const;
    QColor arcFeedColor() const;
    QColor straightFeedColor() const;
    QColor traverseColor() const;
    QColor backplotArcFeedColor() const;
    QColor backplotStraightFeedColor() const;
    QColor backplotTraverseColor() const;
    QColor selectedColor() const;
    QColor activeColor() const;
    QVector3D minimumExtents() const;
    QVector3D maximumExtents() const;
    float lineWidth() const;
    float traverseLineStippleLength() const;
    int arcDivision() const;

public slots:
    virtual void selectDrawable(void *pointer);

    void setModel(GCodeProgramModel * arg);
    void setArcFeedColor(QColor arg);
    void setTraverseColor(QColor arg);
    void setSelectedColor(QColor arg);
    void setStraightFeedColor(QColor arg);
    void setBackplotArcFeedColor(QColor arg);
    void setBackplotStraightFeedColor(QColor arg);
    void setBackplotTraverseColor(QColor arg);
    void setActiveColor(QColor arg);
    void setLineWidth(float arg);
    void setTraverseLineStippleLength(float arg);
    void setArcDivision(int arcDivision);

private:
    struct Position {
        float x;
        float y;
        float z;
        float a;
        float b;
        float c;
        float u;
        float v;
        float w;
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
            drawablePointer(nullptr){}

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
        float helixOffset;     // offset from the rotation axis
        float radius;
        float startAngle;
        float endAngle;
        bool anticlockwise;
        Plane rotationPlane;
    };

    GCodeProgramModel * m_model;
    QColor m_arcFeedColor;
    QColor m_straightFeedColor;
    QColor m_traverseColor;
    QColor m_backplotArcFeedColor;
    QColor m_backplotStraightFeedColor;
    QColor m_backplotTraverseColor;
    QColor m_selectedColor;
    QColor m_activeColor;
    float m_lineWidth;
    float m_traverseLineStippleLength;
    int m_arcDivision;

    Offsets m_activeOffsets;
    machinetalk::Position m_relativePosition; // current relative position (without offsets)
    Position m_currentPosition;  // current absolute position (with offsets)
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
    bool m_extentsUpdated;

    void resetActiveOffsets();
    void resetCurrentPosition();
    void resetRelativePosition();
    void resetActivePlane();
    void resetExtents();
    void updateExtents(const QVector3D &vector);
    void releaseExtents();
    void processPreview(const machinetalk::Preview &preview);
    void processStraightMove(const machinetalk::Preview &preview, MovementType movementType);
    void processArcFeed(const machinetalk::Preview &preview);
    void processSetG5xOffset(const machinetalk::Preview &preview);
    void processSetG92Offset(const machinetalk::Preview &preview);
    void processUseToolOffset(const machinetalk::Preview &preview);
    void processSelectPlane(const machinetalk::Preview &preview);
    Position previewPositionToPosition(const machinetalk::Position &position) const;
    Position calculateNewPosition(const machinetalk::Position &newPosition) const;
    QVector3D positionToVector3D(const Position &position) const;

private slots:
    void drawPath();
    void modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles);
    void triggerFullUpdate();

signals:
    void modelChanged(GCodeProgramModel * arg);
    void arcFeedColorChanged(const QColor &arg);
    void traverseColorChanged(const QColor &arg);
    void selectedColorChanged(const QColor &arg);
    void minimumExtentsChanged(const QVector3D &arg);
    void maximumExtentsChanged(const QVector3D &arg);
    void straightFeedColorChanged(const QColor &arg);
    void executedColorChanged(const QColor &arg);
    void activeColorChanged(const QColor &arg);
    void backplotArcFeedColorChanged(const QColor &arg);
    void backplotStraightFeedColorChanged(const QColor &arg);
    void backplotTraverseColorChanged(const QColor &arg);
    void lineWidthChanged(float lineWidth);
    void traverseLineStippleLengthChanged(float traverseLineStippleLength);
    void arcDivisionChanged(int arcDivision);
}; // class GLPathItem
} // namespace qtquickvcp

#endif // GLPATHITEM_H
