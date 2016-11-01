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

#ifndef GLSPHEREITEM_H
#define GLSPHEREITEM_H

#include "glitem.h"

namespace qtquickvcp {

class GLSphereItem : public GLItem
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    explicit GLSphereItem(QQuickItem *parent = 0);

    virtual void paint(GLView *glView);

    float radius() const
    {
        return m_radius;
    }

    QColor color() const
    {
        return m_color;
    }

    bool isSelected() const
    {
        return m_selected;
    }

signals:
    void radiusChanged(float arg);
    void colorChanged(QColor arg);
    void selectedChanged(bool arg);

public slots:
    virtual void selectDrawable(void *pointer);

    void setRadius(float arg)
    {
        if (m_radius != arg) {
            m_radius = arg;
            emit radiusChanged(arg);
        }
    }

    void setColor(QColor arg)
    {
        if (m_color != arg) {
            m_color = arg;
            emit colorChanged(arg);
        }
    }

private:
    void *m_spherePointer;
    float m_radius;
    QColor m_color;
    bool m_selected;
}; // class GLSphereItem
}; // namespace qtquickvcp

#endif // GLSPHEREITEM_H
