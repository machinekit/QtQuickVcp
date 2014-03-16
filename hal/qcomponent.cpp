#include "qcomponent.h"

QComponent::QComponent(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_updateSocket = NULL;
    m_cmdSocket = NULL;
    m_instanceCount = 0;
    m_heartbeatPeriod = 3000;
    m_name = "Default";
    m_synced = false;
    m_sState = STATE_DOWN;
    m_cState = STATE_DOWN;
    m_ready = false;
    m_containerItem = NULL;

    m_context = createDefaultContext(this);

    m_context->start();
    //m_context->setProperty("Linger", 0);

    m_updateSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_cmdSocket = m_context->createSocket(ZMQSocket::TYP_DEALER, this);
    m_updateSocket->setLinger(0);
    m_cmdSocket->setLinger(0);
    connect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(updateMessageReceived(QList<QByteArray>)));
    connect(m_cmdSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(cmdMessageReceived(QList<QByteArray>)));

    m_heartbeatTimer = new QTimer(this);
    connect(m_heartbeatTimer, SIGNAL(timeout()),
            this, SLOT(hearbeatTimerTick()));
}

void QComponent::componentComplete()
{
    QObjectList halObjects;

    if (m_containerItem == NULL)
    {
        return;
    }

    halObjects = recurseObjects(m_containerItem->children());
    foreach (QObject *object, halObjects)
    {
        QPin *pin = static_cast<QPin *>(object);
        if (pin->name().isEmpty())
        {
            continue;
        }
        m_pinsByName[pin->name()] = pin;
        connect(pin, SIGNAL(valueChanged(QVariant)),
                this, SLOT(pinChange(QVariant)));
#ifdef QT_DEBUG
        qDebug() << "pin added: " << pin->name();
#endif
    }

    QQuickItem::componentComplete();
}

void QComponent::connectToHost()
{
    m_cmdSocket->setIdentity(QString("%1-%2").arg(m_name).arg(QCoreApplication::applicationPid()).toLocal8Bit());

    m_updateSocket->connectTo(m_updateUri);
    m_cmdSocket->connectTo(m_cmdUri);

    m_instanceCount++;

#ifdef QT_DEBUG
    qDebug() << "sockets connected" << m_updateUri << m_cmdUri;
#endif
}

void QComponent::bind()
{
    pb::Component *component;

    m_tx.set_type(pb::MT_HALRCOMP_BIND);
    component = m_tx.add_comp();
    component->set_name(m_name.toStdString());
    foreach (QPin *pin, m_pinsByName)
    {
        pb::Pin *halPin = component->add_pin();
        halPin->set_name(QString("%1.%2").arg(m_name).arg(pin->name()).toStdString());
        halPin->set_type((pb::ValueType)pin->type());
        halPin->set_dir((pb::HalPinDirection)pin->direction());
    }

#ifdef QT_DEBUG
    qDebug() << "bind:" << QString::fromStdString(m_tx.SerializeAsString());
    std::string s;
    gpb::TextFormat::PrintToString(m_tx, &s);
    qDebug() << "bind text:" << QString::fromStdString(s);

#endif

    m_cmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();
}

void QComponent::pinUpdate(pb::Pin remotePin, QPin *localPin)
{
#ifdef QT_DEBUG
    qDebug() << "pinupdate" << localPin->name();
#endif

    if (remotePin.has_halfloat())
    {
        localPin->setValue(QVariant(remotePin.halfloat()));
    }
    else if (remotePin.has_halbit())
    {
        localPin->setValue(QVariant(remotePin.halbit()));
    }
    else if (remotePin.has_hals32())
    {
        localPin->setValue(QVariant(remotePin.hals32()));
    }
    else if (remotePin.has_halu32())
    {
        localPin->setValue(QVariant(remotePin.halu32()));
    }
}

void QComponent::pinChange(QVariant value)
{
    Q_UNUSED(value)
    QPin *pin;
    pb::Pin *halPin;

#ifdef QT_DEBUG
    qDebug() << "pinchange" << m_synced;
#endif

    if (!m_synced)
    {
        return;
    }

    pin = static_cast<QPin *>(QObject::sender());

    if (pin->direction() == QPin::HAL_IN)
    {
        return;
    }

    m_tx.set_type(pb::MT_HALRCOMP_SET);

    // This message MUST carry a Pin message for each pin which has
    // changed value since the last message of this type.
    // Each Pin message MUST carry the handle field.
    // Each Pin message MAY carry the name field.
    // Each Pin message MUST - depending on pin type - carry a halbit,
    // halfloat, hals32, or halu32 field.
    halPin = m_tx.add_pin();

    halPin->set_handle(pin->handle());
    halPin->set_name(QString("%1.%2").arg(m_name).arg(pin->name()).toStdString());
    halPin->set_type((pb::ValueType)pin->type());
    if (pin->type() == QPin::HAL_FLOAT)
    {
        halPin->set_halfloat(pin->value().toDouble());
    }
    else if (pin->type() == QPin::HAL_BIT)
    {
        halPin->set_halbit(pin->value().toBool());
    }
    else if (pin->type() == QPin::HAL_S32)
    {
        halPin->set_hals32(pin->value().toInt());
    }
    else if (pin->type() == QPin::HAL_U32)
    {
        halPin->set_halu32(pin->value().toUInt());
    }

    m_cmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();
}

void QComponent::setReady(bool arg)
{

    {
        if (m_ready != arg) {
            m_ready = arg;
            emit readyChanged(arg);

            if (m_ready)
            {
#ifdef QT_DEBUG
                qDebug() << "ready" << m_name;
#endif
                m_heartbeatTimer->setInterval(m_heartbeatPeriod);
                m_heartbeatTimer->start();
                m_cState = STATE_TRYING;
                emit cStateChanged(m_cState);

                connectToHost();
                bind();
            }
            else
            {
#ifdef QT_DEBUG
                qDebug() << "exit" << m_name;
#endif

                m_instanceCount--;

                if (m_instanceCount == 0)
                {
                    // cleanup here
                    disconnect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
                               this, SLOT(updateMessageReceived(QList<QByteArray>)));
                    disconnect(m_cmdSocket, SIGNAL(messageReceived(QList<QByteArray>)),
                               this, SLOT(cmdMessageReceived(QList<QByteArray>)));
                }
            }
        }
    }
}

QObjectList QComponent::recurseObjects(const QObjectList &list)
{
    QObjectList halObjects;

    foreach (QObject *object, list)
    {
        if (object->inherits("QPin"))
        {
            halObjects.append(object);
        }

        if (object->children().size() > 0)
        {
            halObjects.append(recurseObjects(object->children()));
        }
    }

    return halObjects;
}

void QComponent::updateMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    qDebug() << "status update" << topic << QString::fromStdString(m_rx.SerializeAsString());
#endif

    if (m_rx.type() == pb::MT_HALRCOMP_INCREMENTAL_UPDATE) //incremental update
    {
        for (int i = 0; i < m_rx.pin_size(); ++i)
        {
            pb::Pin remotePin = m_rx.pin(i);
            QPin *localPin = m_pinsByHandle[remotePin.handle()];
            pinUpdate(remotePin, localPin);
        }

        if (m_sState != STATE_UP)
        {
            m_sState = STATE_UP;
            emit sStateChanged(m_sState);
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_FULL_UPDATE)
    {
        for (int i = 0; i < m_rx.comp_size(); ++i)
        {
            pb::Component component = m_rx.comp(i);
            for (int j = 0; j < component.pin_size(); j++)
            {
                pb::Pin remotePin = component.pin(j);
                QString name = QString::fromStdString(remotePin.name());
                if (name.indexOf(".") != -1)    // strip comp prefix
                {
                    name = name.mid(name.indexOf(".")+1);
                }
                QPin *localPin = m_pinsByName[name];
                localPin->setHandle(remotePin.handle());
                m_pinsByHandle[remotePin.handle()] = localPin;
                pinUpdate(remotePin, localPin);
            }
            m_synced = true;
            emit syncedChanged(m_synced);

            if (m_sState != STATE_UP)
            {
                m_sState = STATE_UP;
                emit sStateChanged(m_sState);
            }
        }

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMMAND_ERROR)
    {
        QStringList notes;

        for (int i = 0; i < m_rx.note_size(); ++i)
        {
            notes.append(QString::fromStdString(m_rx.note(i)));
        }
#ifdef QT_DEBUG
        qDebug() << "proto error on subscribe" << notes;
#endif
        emit protocolError(notes);

        m_sState = STATE_DOWN;
        emit sStateChanged(STATE_DOWN);

        return;
    }

#ifdef QT_DEBUG
        qDebug() << "status_update: unknown message type: " << QString::fromStdString(m_rx.SerializeAsString());
#endif
}

void QComponent::cmdMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    //topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(0).data(), messageList.at(0).size());

#ifdef QT_DEBUG
    qDebug() << "server message" << topic << QString::fromStdString(m_rx.SerializeAsString());
#endif

    if (m_rx.type() == pb::MT_PING_ACKNOWLEDGE)
    {
        m_cState = STATE_UP;
        emit cStateChanged(m_cState);
        m_pingOutstanding = false;

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_BIND_CONFIRM)
    {
#ifdef QT_DEBUG
        qDebug() << "bind confirmed";
#endif
        m_cState = STATE_UP;
        m_sState = STATE_TRYING;
        emit cStateChanged(m_cState);
        emit sStateChanged(m_sState);
        m_updateSocket->subscribeTo(m_name.toLocal8Bit());

        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_BIND_REJECT)
    {
        m_cState = STATE_DOWN;
        emit cStateChanged(m_cState);
#ifdef QT_DEBUG
        qDebug() << "bind rejected" << QString::fromStdString(m_rx.note(0));
#endif
        return;
    }
    else if (m_rx.type() == pb::MT_HALRCOMP_SET_REJECT)
    {
        m_cState = STATE_DOWN;
        emit cStateChanged(m_cState);
#ifdef QT_DEBUG
        qDebug() << "pin change rejected" << QString::fromStdString(m_rx.note(0));
#endif
        return;
    }
    else
    {
#ifdef QT_DEBUG
        qDebug() << "UNKNOWN server message type";
#endif
    }

}

void QComponent::hearbeatTimerTick()
{
    if (m_pingOutstanding)
    {
        m_cState = STATE_TRYING;
        emit cStateChanged(m_cState);
#ifdef QT_DEBUG
        qDebug() << "timeout";
#endif
    }

    m_tx.set_type(pb::MT_PING);

    m_cmdSocket->sendMessage(QByteArray(m_tx.SerializeAsString().c_str(), m_tx.ByteSize()));
    m_tx.Clear();

    m_pingOutstanding = true;
}

