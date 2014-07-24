#include "qglobjectdimensionsitem.h"

QGLObjectDimensionsItem::QGLObjectDimensionsItem(QQuickItem *parent) :
    QGLItem(parent),
    m_objectSize(QVector3D(2.0, 3.0, 2.5)),
    m_objectPosition(QVector3D(1.0, 0.8, 0.0)),
    m_color(QColor(Qt::magenta)),
    m_textSize(0.5),
    m_prefix(QString()),
    m_suffix(QString()),
    m_decimals(1)
{
    connect(this, SIGNAL(objectSizeChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(objectPositionChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(textSizeChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(prefixChanged(QString)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(suffixChanged(QString)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(decimalsChanged(int)),
            this, SIGNAL(propertyChanged()));
}

void QGLObjectDimensionsItem::paint(QGLView *glView)
{
    float lineEnding = 0.4;
    float lineOffset = 0.5;
    float textOffset = 0.2;

    glView->translate(position());
    glView->rotate(rotation());
    glView->scale(scale());

    glView->color(m_color);
    glView->beginUnion(modelId());

        glView->translate(m_objectPosition.x(), -lineOffset, 0.0);
        glView->beginUnion(modelId());
            glView->translate(0.0, lineEnding/2, 0.0);
            glView->line(0.0, -lineEnding, 0.0);
            glView->line(m_objectSize.x(), 0.0, 0.0);
            glView->translate(m_objectSize.x(), lineEnding/2, 0.0);
            glView->line(0.0 , -lineEnding, 0.0);
            glView->translate(m_objectSize.x()/2.0, -textOffset - m_textSize, 0.0);
            glView->scale(m_textSize, m_textSize, m_textSize);
            glView->text(m_prefix + QString::number(m_objectSize.x(), 'g', m_decimals) + m_suffix, QGLView::AlignCenter);
        glView->endUnion();

        glView->rotate(90, 0, 0, 1);
        glView->translate(m_objectPosition.y(), lineOffset, 0.0);
        glView->beginUnion(modelId());
            glView->translate(0.0, lineEnding/2, 0.0);
            glView->line(0.0, -lineEnding, 0.0);
            glView->line(m_objectSize.y(), 0.0, 0.0);
            glView->translate(m_objectSize.y(), lineEnding/2, 0.0);
            glView->line(0.0 , -lineEnding, 0.0);
        glView->endUnion();

        glView->rotate(-90, 0, 1, 0);
        glView->translate(m_objectPosition.z(), -lineOffset, 0.0);
        glView->beginUnion(modelId());
            glView->translate(0.0, lineEnding/2, 0.0);
            glView->line(0.0, -lineEnding, 0.0);
            glView->line(m_objectSize.z(), 0.0, 0.0);
            glView->translate(m_objectSize.z(), lineEnding/2, 0.0);
            glView->line(0.0 , -lineEnding, 0.0);
        glView->endUnion();

    glView->endUnion();
}
