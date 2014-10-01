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

#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include "qglitem.h"

class QGLCanvas : public QGLItem
{
    Q_OBJECT
    Q_PROPERTY(QGLView *context READ contex NOTIFY contextChanged)

public:
    explicit QGLCanvas(QQuickItem *parent = 0);

    virtual void paint(QGLView *glView);

    QGLView * contex() const
    {
        return m_context;
    }

signals:
    void contextChanged(QGLView * arg);
    void paint();
    void drawableSelected(void *pointer);

public slots:
    virtual void selectDrawable(void *pointer);

private:
    QGLView * m_context;
};

#endif // QGLCANVAS_H
