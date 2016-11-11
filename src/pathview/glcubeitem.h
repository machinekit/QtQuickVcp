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

#ifndef GLCUBEITEM_H
#define GLCUBEITEM_H

#include "glitem.h"

namespace qtquickvcp {

class GLCubeItem : public GLItem
{
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool centered READ isCentered WRITE setCentered NOTIFY centeredChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    explicit GLCubeItem(QQuickItem *parent = 0);

    virtual void paint(GLView *glView);

    QVector3D size() const
    {
        return m_size;
    }

    QColor color() const
    {
        return m_color;
    }

    bool isCentered() const
    {
        return m_centered;
    }

    bool isSelected() const
    {
        return m_selected;
    }

signals:
    void sizeChanged(QVector3D arg);
    void colorChanged(QColor arg);
    void centeredChanged(bool arg);

    void selectedChanged(bool arg);

public slots:
    virtual void selectDrawable(void *pointer);

    void setSize(float w, float l, float h)
    {
        setSize(QVector3D(w,l,h));
    }

    void setSize(QVector3D arg)
    {
        if (m_size != arg) {
            m_size = arg;
            emit sizeChanged(arg);
        }
    }

    void setColor(QColor arg)
    {
        if (m_color != arg) {
            m_color = arg;
            emit colorChanged(arg);
        }
    }

    void setCentered(bool arg)
    {
        if (m_centered != arg) {
            m_centered = arg;
            emit centeredChanged(arg);
        }
    }

private:
    void* m_cubePointer;

    QVector3D m_size;
    QColor m_color;
    bool m_centered;
    bool m_selected;
}; // GLCubeItem
} // namespace qtquickvcp

#endif // GLCUBEITEM_H
