#ifndef QCOMPONENT_H
#define QCOMPONENT_H

#include <QQuickItem>
#include "qpin.h"
#include <nzmqt/nzmqt.hpp>
//#include "message.pb.h"
//#include <sdiscover.h>

using namespace nzmqt;

class QComponent : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QComponent(QQuickItem *parent = 0);

    virtual void componentComplete();

signals:

public slots:

private:
    ZMQContext *m_context;
    ZMQSocket  *m_updateSocket;
    ZMQSocket  *m_cmdSocket;
    int         m_instanceCount;

    QObjectList recurseObjects(const QObjectList &list);

};

#endif // QCOMPONENT_H
