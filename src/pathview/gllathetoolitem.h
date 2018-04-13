#ifndef GLLATHETOOLITEM_H
#define GLLATHETOOLITEM_H

#include "glitem.h"

namespace qtquickvcp {

class GLLatheToolItem : public GLItem
{
    Q_OBJECT
    Q_PROPERTY(QColor crossColor READ crossColor WRITE setCrossColor NOTIFY crossColorChanged)
    Q_PROPERTY(QColor shapeColor READ shapeColor WRITE setShapeColor NOTIFY shapeColorChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(float frontangle READ frontangle WRITE setFrontangle NOTIFY frontangleChanged)
    Q_PROPERTY(float backangle READ backangle WRITE setBackangle NOTIFY backangleChanged)
    Q_PROPERTY(float diameter READ diameter WRITE setDiameter NOTIFY diameterChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    GLLatheToolItem(QQuickItem *parent = nullptr);

    void paint(GLView *glView);

    QColor crossColor() const;

    QColor shapeColor() const;

    int orientation() const;

    float frontangle() const;

    float backangle() const;

    float diameter() const;

    bool isSelected() const;

public slots:
    void selectDrawable(void *pointer);

    void setCrossColor(const QColor& crossColor);

    void setOrientation(int orientation);

    void setFrontangle(float frontangle);

    void setBackangle(float backangle);

    void setDiameter(float diameter);

    void setShapeColor(const QColor& shapeColor);

signals:
    void drawableSelected(void *pointer);
    void crossColorChanged(QColor crossColor);
    void orientationChanged(int orientation);
    void frontangleChanged(float frontangle);
    void backangleChanged(float backangle);
    void diameterChanged(float diameter);
    void selectedChanged(bool selected);

    void shapeColorChanged(QColor shapeColor);

private:
    void *m_polygonPointer;

    QColor m_crossColor;
    QColor m_shapeColor;
    int m_orientation;
    float m_frontangle;
    float m_backangle;
    float m_diameter;
    bool m_selected;

    void paintToolShape(GLView *glView);

}; // GLLatheToolItem
} // namespace qtquickvcp

#endif // GLLATHETOOLITEM_H
