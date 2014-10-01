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

#ifndef QGLLIGHT_H
#define QGLLIGHT_H

#include <QObject>
#include <QVector3D>

class QGLLight : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D intensities READ intensities WRITE setIntensities NOTIFY intensitiesChanged)
    Q_PROPERTY(float attenuation READ attenuation WRITE setAttenuation NOTIFY attenuationChanged)
    Q_PROPERTY(float ambientCoefficient READ ambientCoefficient WRITE setAmbientCoefficient NOTIFY ambientCoefficientChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit QGLLight(QObject *parent = 0);

    QVector3D position() const
    {
        return m_position;
    }

    QVector3D intensities() const
    {
        return m_intensities;
    }

    float attenuation() const
    {
        return m_attenuation;
    }

    bool enabled() const
    {
        return m_enabled;
    }

    float ambientCoefficient() const
    {
        return m_ambientCoefficient;
    }

signals:
    void positionChanged(QVector3D arg);
    void intensitiesChanged(QVector3D arg);
    void attenuationChanged(float arg);
    void enabledChanged(bool arg);
    void ambientCoefficientChanged(float arg);
    void propertyChanged();

public slots:
    void setPosition(QVector3D arg)
    {
        if (m_position != arg) {
            m_position = arg;
            emit positionChanged(arg);
        }
    }

    void setIntensities(QVector3D arg)
    {
        if (m_intensities != arg) {
            m_intensities = arg;
            emit intensitiesChanged(arg);
        }
    }

    void setAttenuation(float arg)
    {
        if (m_attenuation != arg) {
            m_attenuation = arg;
            emit attenuationChanged(arg);
        }
    }

    void setEnabled(bool arg)
    {
        if (m_enabled != arg) {
            m_enabled = arg;
            emit enabledChanged(arg);
        }
    }

    void setAmbientCoefficient(float arg)
    {
        if (m_ambientCoefficient != arg) {
            m_ambientCoefficient = arg;
            emit ambientCoefficientChanged(arg);
        }
    }

private:
    QVector3D m_position;
    QVector3D m_intensities;
    float m_attenuation;
    bool m_enabled;
    float m_ambientCoefficient;
};

#endif // QGLLIGHT_H
