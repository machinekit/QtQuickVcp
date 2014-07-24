#include "qgllight.h"

QGLLight::QGLLight(QObject *parent) :
    QObject(parent),
    m_position(QVector3D(1.0, 1.0, 5.0)),
    m_intensities(QVector3D(1.0, 1.0, 1.0)),
    m_attenuation(0.01),
    m_enabled(true),
    m_ambientCoefficient(0.1)
{
    connect(this, SIGNAL(positionChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(intensitiesChanged(QVector3D)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(attenuationChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(ambientCoefficientChanged(float)),
            this, SIGNAL(propertyChanged()));
    connect(this, SIGNAL(enabledChanged(bool)),
            this, SIGNAL(propertyChanged()));
}
