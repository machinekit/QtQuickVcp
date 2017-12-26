#include "halremotecomponent.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace machinetalk;

namespace qtquickvcp {
/*!
    \qmltype HalRemoteComponent
    \instantiates qtquickvcp::HalRemoteComponent
    \inqmlmodule Machinekit.HalRemote
    \brief A HAL remote component.
    \ingroup halremote

    This component provides the counterpart of a HAL
    remote component in the HAL real-time environment.
    The HalRemoteComponent connects to a remote HAL instance
    using the halrcmd and halrcomp services provided by
    a Haltalk instance running on the remote host.

    A HalRemoteComponent needs the \l{halrcmdUri},
    \l{halrcompUri} and \l containerItem set in order
    to work.

    The HalRemoteComponent scans the \l containerItem
    and its children for \l{HalPin}s when \l ready is set
    to \c true.

    The following example creates a HAL remote component
    \c myComponent with one pin \c myPin. The resulting
    name for the pin inside HAL is \c myComponent.myPin.

    \qml
    Item {
        Item {
            id: container

            HalPin {
                name: "myPin"
                type: HalPin.Float
                direction: HalPin.Out
            }
        }

        HalRemoteComponent {
            id: halRemoteComponent

            name: "myComponent"
            halrcmdUri: "tcp://192.168.1.2:5001"
            halrcompUri: "tcp://192.168.1.2:5002"
            containerItem: container
            ready: true
        }
    }
    \endqml
*/

/*! \qmlproperty string HalRemoteComponent::halrcmdUri

    This property holds the halrcmd service uri.
*/

/*! \qmlproperty string HalRemoteComponent::halrcompUri

    This property holds the halrcomp service uri.
*/

/*! \qmlproperty string HalRemoteComponent::name

    This property holds the name of the remote component.
*/

/*! \qmlproperty int HalRemoteComponent::halrcmdHeartbeatInterval

    This property holds the period time of the heartbeat timer in ms.
    Set this property to \c{0} to disable the hearbeat.

    The default value is \c{3000}.
*/

/*! \qmlproperty int HalRemoteComponent::halrcompHeartbeatInterval

    This property holds the period time of the heartbeat timer in ms.
    This value is set automatically on the first full update.

    The default value is \c{0}.
*/

/*! \qmlproperty bool HalRemoteComponent::ready

    This property holds whether the HalRemoteComponent is ready or not.
    If the property is set to \c true the component will try to connect. If the
    property is set to \c false all connections will be closed.

    The default value is \c{false}.
*/

/*! \qmlproperty bool HalRemoteComponent::connected

    This property holds wheter the HAL remote component is connected or not. This is the
    same as \l{connectionState} == \c{HalRemoteComponent.Connected}.
 */

/*! \qmlproperty enumeration HalRemoteComponent::connectionState

    This property holds the connection state of the HAL remote component.

    \list
    \li HalRemoteComponent.Disconnected - The component is not connected.
    \li HalRemoteComponent.Connecting - The component is trying to connect to the remote component.
    \li HalRemoteComponent.Connected - The component is connected and pin changes are accepted.
    \li HalRemoteComponent.Error - An error has happened. See \l error and \l errorString for details about the error.
    \endlist
*/

/*! \qmlproperty enumeration HalRemoteComponent::error

    This property holds the currently active error. See \l errorString
    for a description of the active error.

    \list
    \li HalRemoteComponent.NoError - No error happened.
    \li HalRemoteComponent.ComponentError - An error happened.
    \endlist

    \sa errorString
*/

/*! \qmlproperty string HalRemoteComponent::errorString

    This property holds a text description of the last error that occured.
    If \l error holds a error value check this property for the description.

    \sa error
*/

/*! \qmlproperty Item HalRemoteComponent::containerItem

    This property holds the item that should be scanned for
    \l{HalPin}s.

    The default value is \c{nullptr}.
*/

/*! \qmlproperty Item HalRemoteComponent::create

    Specifies wether the component should be created on bind if it
    does not exist on the remote host.

    The default value is \c{true}.
*/

/*! \qmlproperty Item HalRemoteComponent::bind

    Specifies wether the component should be bound when connecting. If not the
    remote representation of the pins is refrelcted by \c{pins}.

    The default value is \c{true}.
*/

/*! \qmlproperty list<HalPin> HalRemoteComponent::pins

    This property holds a list of HAL pins when bound or connected.
*/

/** Remote HAL Component implementation for use with C++ and QML */
HalRemoteComponent::HalRemoteComponent(QObject *parent) :
    halremote::RemoteComponentBase(parent),
    m_name("default"),
    m_connected(false),
    m_error(NoError),
    m_containerItem(this),
    m_create(true),
    m_bind(true)
{
}

/** Updates a remote pin witht the value of a local pin */
void HalRemoteComponent::pinChange(QVariant value)
{
    Q_UNUSED(value)
    HalPin *pin;
    Pin *halPin;

    if (state() != State::Synced) // only accept pin changes if we are connected
    {
        return;
    }

    pin = static_cast<HalPin *>(QObject::sender());

    if (pin->direction() == HalPin::In)   // Only update Output or IO pins
    {
        return;
    }

#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name,  "pin change" << pin->name() << pin->value())
#endif

    // This message MUST carry a Pin message for each pin which has
    // changed value since the last message of this type.
    // Each Pin message MUST carry the handle field.
    // Each Pin message MAY carry the name field.
    // Each Pin message MUST carry the type field
    // Each Pin message MUST - depending on pin type - carry a halbit,
    // halfloat, hals32, or halu32 field.
    halPin = m_tx.add_pin();

    halPin->set_handle(static_cast<gpb::uint32>(pin->handle()));
    halPin->set_type(static_cast<ValueType>(pin->type()));
    if (pin->type() == HalPin::Float)
    {
        halPin->set_halfloat(pin->value().toDouble());
    }
    else if (pin->type() == HalPin::Bit)
    {
        halPin->set_halbit(pin->value().toBool());
    }
    else if (pin->type() == HalPin::S32)
    {
        halPin->set_hals32(pin->value().toInt());
    }
    else if (pin->type() == HalPin::U32)
    {
        halPin->set_halu32(pin->value().toUInt());
    }

    sendHalrcompSet(m_tx);
}

/** Recurses through a list of objects */
const QObjectList HalRemoteComponent::recurseObjects(const QObjectList &list)
{
    QObjectList halObjects;

    for (QObject *object: list)
    {
        HalPin *halPin = qobject_cast<HalPin *>(object);
        if (halPin != nullptr)
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

/** Updates a local pin with the value of a remote pin */
void HalRemoteComponent::pinUpdate(const Pin &remotePin, HalPin *localPin)
{
#ifdef QT_DEBUG
    DEBUG_TAG(2, m_name,  "pin update" << localPin->name() << remotePin.halfloat() << remotePin.halbit() << remotePin.hals32() << remotePin.halu32())
#endif

    if (remotePin.has_halfloat())
    {
        localPin->setValue(QVariant(remotePin.halfloat()), true);
    }
    else if (remotePin.has_halbit())
    {
        localPin->setValue(QVariant(remotePin.halbit()), true);
    }
    else if (remotePin.has_hals32())
    {
        localPin->setValue(QVariant(remotePin.hals32()), true);
    }
    else if (remotePin.has_halu32())
    {
        localPin->setValue(QVariant(remotePin.halu32()), true);
    }
}

/** Adds a local pin based on remote pin representation **/
HalPin *HalRemoteComponent::addLocalPin(const Pin &remotePin)
{
    QString name = QString::fromStdString(remotePin.name());
    name = splitPinFromHalName(name);
    HalPin *localPin = new HalPin(this);
    localPin->setName(name);
    localPin->setType(static_cast<HalPin::HalPinType>(remotePin.type()));
    localPin->setDirection(static_cast<HalPin::HalPinDirection>(remotePin.dir()));
    m_pinsByName[name] = localPin;
    m_pins.append(localPin);
    connect(localPin, &HalPin::valueChanged,
            this, &HalRemoteComponent::pinChange);

    return localPin;
}

/** Generates a Bind messages and sends it over the suitable 0MQ socket */
void HalRemoteComponent::bindPins()
{
    Component *component;

    component = m_tx.add_comp();
    component->set_name(m_name.toStdString());
    component->set_no_create(!m_create);
    for (HalPin *pin: qAsConst(m_pinsByName))
    {
        Pin *halPin = component->add_pin();
        halPin->set_name(QString("%1.%2").arg(m_name, pin->name()).toStdString());  // pin name is always component.name
        halPin->set_type(static_cast<ValueType>(pin->type()));
        halPin->set_dir(static_cast<HalPinDirection>(pin->direction()));
        if (pin->type() == HalPin::Float)
        {
            halPin->set_halfloat(pin->value().toDouble());
        }
        else if (pin->type() == HalPin::Bit)
        {
            halPin->set_halbit(pin->value().toBool());
        }
        else if (pin->type() == HalPin::S32)
        {
            halPin->set_hals32(pin->value().toInt());
        }
        else if (pin->type() == HalPin::U32)
        {
            halPin->set_halu32(pin->value().toUInt());
        }
    }

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(m_tx, &s);
    DEBUG_TAG(1, m_name, "bind");
    DEBUG_TAG(3, m_name, QString::fromStdString(s));
#endif

    sendHalrcompBind(m_tx);
}

/** splits HAL pin name from full HAL name **/
QString HalRemoteComponent::splitPinFromHalName(const QString &name)
{
    QString newName;
    int dotIndex = name.indexOf(".");
    if (dotIndex != -1)    // strip comp prefix
    {
        newName = name.mid(dotIndex + 1);
    }
    else
    {
        newName = name;
    }
    return newName;
}

/** Scans all children of the container item for pins and adds them to a map */
void HalRemoteComponent::addPins()
{
    if (m_containerItem == nullptr)
    {
        return;
    }

    clearHalrcompTopics();
    addHalrcompTopic(m_name.toLocal8Bit());

    const auto halObjects = recurseObjects(m_containerItem->children());
    for (QObject *object: halObjects)
    {
        HalPin *pin = static_cast<HalPin *>(object);
        if (pin->name().isEmpty()  || (pin->enabled() == false))    // ignore pins with empty name and disabled pins
        {
            continue;
        }
        m_pinsByName[pin->name()] = pin;
        m_pins.append(pin);
        connect(pin, SIGNAL(valueChanged(QVariant)),
                this, SLOT(pinChange(QVariant)));
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_name, "pin added: " << pin->name())
#endif
    }

    emit pinsChanged(pins());
}

/** Removes all previously added pins */
void HalRemoteComponent::removePins()
{
    for (HalPin *pin: qAsConst(m_pinsByName))
    {
        disconnect(pin, &HalPin::valueChanged,
                this, &HalRemoteComponent::pinChange);

        if (pin->parent() == this) // pin was created by this class
        {
            pin->deleteLater();
        }
    }

    m_pinsByHandle.clear();
    m_pinsByName.clear();
    m_pins.clear();
    emit pinsChanged(pins());
}

/** Sets synced of all pins to false */
void HalRemoteComponent::unsyncPins()
{
    for (HalPin *pin: qAsConst(m_pinsByName)) {
        pin->setSynced(false);
    }
}

void HalRemoteComponent::handleHalrcompFullUpdateMessage(const QByteArray &topic,const Container &rx)
{
    Q_UNUSED(topic);
    bool pinsAdded = false;

    if (rx.comp_size() == 0) // empty message
    {
        return;
    }

    const Component &component = rx.comp(0);  // shouldnt we check the name?
    for (const Pin &remotePin: component.pin())
    {
        QString name = QString::fromStdString(remotePin.name());
        name = splitPinFromHalName(name);

        HalPin *localPin = m_pinsByName.value(name, nullptr);
        if (localPin == nullptr)
        {
            localPin = addLocalPin(remotePin);
            pinsAdded = true;
        }

        localPin->setHandle(static_cast<int>(remotePin.handle()));
        m_pinsByHandle.insert(static_cast<int>(remotePin.handle()), localPin);
        pinUpdate(remotePin, localPin);
    }

    if (pinsAdded)
    {
        emit pinsChanged(pins());
    }

    pinsSynced(); // accept that pins have been synced
}

void HalRemoteComponent::handleHalrcompIncrementalUpdateMessage(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);

    for (const Pin &remotePin: rx.pin())
    {
        HalPin *localPin = m_pinsByHandle.value(static_cast<int>(remotePin.handle()), nullptr);
        if (localPin != nullptr) // in case we received a wrong pin handle
        {
            pinUpdate(remotePin, localPin);
        }
    }
}

void HalRemoteComponent::handleHalrcompErrorMessage(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    Q_UNUSED(rx);
    // errorString is automatically updated
}

void HalRemoteComponent::bindComponent()
{
    if (m_bind)
    {
        bindPins();
    }
    else
    {
       noBind();
    }
}

void HalRemoteComponent::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void HalRemoteComponent::setError()
{
    if (m_error != ComponentError) {
        m_error = ComponentError;
        emit errorChanged(m_error);
    }

    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void HalRemoteComponent::setDisconnected()
{
    if (m_error != NoError) {
        m_error = NoError;
        emit errorChanged(m_error);
    }

    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void HalRemoteComponent::setConnecting()
{
    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void HalRemoteComponent::setTimeout()
{
    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

} // namespace qtquickvcp
