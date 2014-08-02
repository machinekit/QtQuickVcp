#ifndef QGLPATHITEM_H
#define QGLPATHITEM_H

#include "qglitem.h"
#include "qgcodeprogrammodel.h"
#include "preview.pb.h"

class QGLPathItem : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QColor feedColor READ feedColor WRITE setFeedColor NOTIFY feedColorChanged)
    Q_PROPERTY(QColor traverseColor READ traverseColor WRITE setTraverseColor NOTIFY traverseColorChanged)
    Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit QGLPathItem(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QGCodeProgramModel * model() const;
    QColor feedColor() const;
    QColor traverseColor() const;
    QColor selectedColor() const;

public slots:
    virtual void selectDrawable(void *pointer);

    void setModel(QGCodeProgramModel * arg);
    void setFeedColor(QColor arg);
    void setTraverseColor(QColor arg);
    void setSelectedColor(QColor arg);

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
        ArcPathItem(): PathItem() {
            pathType = Arc;
        }
    };

    QGCodeProgramModel * m_model;
    QColor m_feedColor;
    QColor m_traverseColor;
    QColor m_selectedColor;

    Offsets m_activeOffsets;
    Position m_currentPosition;
    QList<PathItem*> m_previewPathItems;
    QModelIndex m_currentModelIndex;
    QMap<QModelIndex, PathItem*> m_modelPathMap;  // for mapping the model to internal items
    QMap<void*, PathItem*> m_drawablePathMap;  // for mapping GL views drawables to internal items
    void* m_previousSelectedDrawable;

    bool m_needsFullUpdate;
    QList<PathItem*> m_modifiedPathItems;

    void resetActiveOffsets();
    void resetCurrentPosition();
    void processPreview(const pb::Preview &preview);
    void processStraightMove(const pb::Preview &preview, MovementType movementType);
    void processArcFeed(const pb::Preview &preview);
    void processSetG5xOffset(const pb::Preview &preview);
    void processSetG92Offset(const pb::Preview &preview);
    void processUseToolOffset(const pb::Preview &preview);
    Position previewPositionToPosition(const pb::Position &position) const;
    Position calculateNewPosition(const pb::Position &newPosition) const;
    QVector3D positionToVector3D(const Position &position) const;

private slots:
    void drawPath();
    void modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles);

signals:
    void modelChanged(QGCodeProgramModel * arg);
    void feedColorChanged(QColor arg);
    void traverseColorChanged(QColor arg);
    void selectedColorChanged(QColor arg);
};

#endif // QGLPATHITEM_H
