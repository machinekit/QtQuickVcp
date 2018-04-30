#include "gllathetoolitem.h"

#include <cmath>
#include "pi_constants.h"

namespace qtquickvcp {

GLLatheToolItem::GLLatheToolItem(QQuickItem *parent) :
    GLItem(parent),
    m_polygonPointer(nullptr),
    m_crossColor(QColor(Qt::darkRed)),
    m_shapeColor(QColor(Qt::white)),
    m_orientation(0),
    m_frontangle(0.0f),
    m_backangle(0.0f),
    m_diameter(0.0f),
    m_selected(false)
{
    connect(this, &GLLatheToolItem::crossColorChanged,
            this, &GLLatheToolItem::needsUpdate);
    connect(this, &GLLatheToolItem::orientationChanged,
            this, &GLLatheToolItem::needsUpdate);
    connect(this, &GLLatheToolItem::frontangleChanged,
            this, &GLLatheToolItem::needsUpdate);
    connect(this, &GLLatheToolItem::backangleChanged,
            this, &GLLatheToolItem::needsUpdate);
    connect(this, &GLLatheToolItem::diameterChanged,
            this, &GLLatheToolItem::needsUpdate);
}

void GLLatheToolItem::paintToolShape(GLView *glView)
{
    const float LATHE_SHAPES [][2] = {
        {0.0f, 0.0f}, // 0
        {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}, {-1.0f, -1.0f}, // 1..4
        {0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f}, // 5..8
        {0.0f, 0.0f} // 9
    };

    QVector<QVector3D> points;
    const float w = 3.0f / 8.0f;
    const float radius = m_diameter / 2.0f;

    // paint cross
    glView->color(m_crossColor);
    glView->lineFromTo(
                QVector3D(-radius / 2.0f, 0.0f, 0.0f),
                QVector3D(radius / 2.0f, 0.0f, 0.0f)
    );
    glView->color(m_crossColor);
    glView->lineFromTo(
                QVector3D(0.0f, 0.0f, -radius / 2.0f),
                QVector3D(0.0f, 0.0f, radius / 2.0f)
    );

    if (m_orientation == 9) {
        points.append(QVector3D(0.0f, 0.0f, 0.0f));
        for (int i = 0; i < 37; ++i) {
            float t = static_cast<float>(i) * PI_F / 18.0f;
            points.append(QVector3D(radius * std::cos(t), 0.0f, radius * std::sin(t)));
        }
    }
    else if (0 < m_orientation && m_orientation < 9) {
        float dx = LATHE_SHAPES[m_orientation][0];
        float dy = LATHE_SHAPES[m_orientation][1];

        float minAngle = qMin(m_backangle, m_frontangle) * PI_F / 180.0f;
        float maxAngle = qMax(m_backangle, m_frontangle) * PI_F / 180.0f;

        float sinMax = std::sin(maxAngle);
        float cosMax = std::cos(maxAngle);
        float sinMin = std::sin(minAngle);
        float cosMin = std::cos(minAngle);

        float circleMinAngle = - (PI_F/2) + minAngle;
        float circleMaxAngle = - 3*(PI_F/2) + maxAngle;

        float sz = qMax(w, 3*radius);

        // first fin
        points.append(QVector3D(radius * dx, 0.0f, radius * dy));
        points.append(QVector3D(
            radius * dx + radius * std::sin(circleMinAngle) + sz * sinMin,
            0.0f,
            radius * dy + radius * std::cos(circleMinAngle) + sz * cosMin
        ));
        // half circle
        for (int i = 0; i < 37; ++i) {
            float t = circleMinAngle + i * (circleMaxAngle - circleMinAngle) / 36.0f;
            points.append(QVector3D(radius * dx + radius * std::sin(t),
                                    0.0f,
                                    radius * dy + radius * std::cos(t)));
        }
        // second fin
        points.append(QVector3D(
            radius * dx + radius * std::sin(circleMaxAngle) + sz * sinMax,
            0.0f,
            radius * dy + radius * std::cos(circleMaxAngle) + sz * cosMax
        ));
        // close
        points.append(QVector3D(
            radius * dx + radius * std::sin(circleMinAngle) + sz * sinMin,
            0.0f,
            radius * dy + radius * std::cos(circleMinAngle) + sz * cosMin
        ));
    }
    else {
        qWarning("Unsupported orientation passed to lathe tool item.");
    }

    glView->color(m_shapeColor);
    glView->polygon(points);
}

void GLLatheToolItem::paint(GLView *glView)
{
    glView->prepare(this);

    glView->reset();
    glView->beginUnion();
    paintToolShape(glView);
    glView->endUnion();
}

void GLLatheToolItem::selectDrawable(void *pointer)
{
    if (m_polygonPointer == pointer) {
        return;
    }

    bool selected = (pointer == m_polygonPointer);

    if (selected != m_selected) {
        m_selected = selected;
        emit selectedChanged(m_selected);
    }
}

QColor GLLatheToolItem::crossColor() const
{
    return m_crossColor;
}

QColor GLLatheToolItem::shapeColor() const
{
    return m_shapeColor;
}

bool GLLatheToolItem::isSelected() const
{
    return m_selected;
}

int GLLatheToolItem::orientation() const
{
    return m_orientation;
}

float GLLatheToolItem::frontangle() const
{
    return m_frontangle;
}

float GLLatheToolItem::backangle() const
{
    return m_backangle;
}

float GLLatheToolItem::diameter() const
{
    return m_diameter;
}

void GLLatheToolItem::setCrossColor(const QColor &color)
{
    if (m_crossColor == color) {
        return;
    }

    m_crossColor = color;
    emit crossColorChanged(m_crossColor);
}

void GLLatheToolItem::setOrientation(int orientation)
{
    if (m_orientation == orientation) {
        return;
    }

    m_orientation = orientation;
    emit orientationChanged(m_orientation);
}

void GLLatheToolItem::setFrontangle(float frontangle)
{
    if (qFuzzyCompare(m_frontangle, frontangle)) {
        return;
    }

    m_frontangle = frontangle;
    emit frontangleChanged(m_frontangle);
}

void GLLatheToolItem::setBackangle(float backangle)
{
    if (qFuzzyCompare(m_backangle, backangle)) {
        return;
    }

    m_backangle = backangle;
    emit backangleChanged(m_backangle);
}

void GLLatheToolItem::setDiameter(float diameter)
{
    if (qFuzzyCompare(m_diameter, diameter)) {
        return;
    }

    m_diameter = diameter;
    emit diameterChanged(m_diameter);
}

void GLLatheToolItem::setShapeColor(const QColor &shapeColor)
{
    if (m_shapeColor == shapeColor) {
        return;
    }

    m_shapeColor = shapeColor;
    emit shapeColorChanged(m_shapeColor);
}

} // namespace qtquickvcp
