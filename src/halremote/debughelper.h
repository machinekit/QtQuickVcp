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

#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#ifdef QT_DEBUG

const int debugLevel = 1;

#define DEBUG(l, x) if (debugLevel >= l) qDebug() << x;
#define DEBUG_TAG(l, tag, x) if (debugLevel >= l) qDebug() << "[" << tag << "]" << x;
#define WARNING(l, x) if (debugLevel >= l) qWarning() << x;
#define WARNING_TAG(l, tag, x) if (debugLevel >= l) qWarning() << "[" << tag << "]" << x;

#endif

#endif // DEBUGHELPER_H
