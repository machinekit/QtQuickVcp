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

#include "gllight.h"

namespace qtquickvcp {

GLLight::GLLight(QObject *parent) :
    QObject(parent),
    m_position(QVector3D(1.0, 1.0, 5.0)),
    m_intensities(QVector3D(1.0, 1.0, 1.0)),
    m_attenuation(0.01),
    m_enabled(true),
    m_ambientCoefficient(0.1)
{
    connect(this, &GLLight::positionChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::intensitiesChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::attenuationChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::ambientCoefficientChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::enabledChanged,
            this, &GLLight::propertyChanged);
}
} // namespace qtquickvcp
