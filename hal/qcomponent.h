#ifndef QCOMPONENT_H
#define QCOMPONENT_H

#include <QQuickItem>
#include <QHash>
#include <QTimer>
#include "qpin.h"
#include <nzmqt/nzmqt.hpp>
#include "message.pb.h"

using namespace nzmqt;

class QComponent : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString cmdUri READ cmdUri WRITE setCmdUri NOTIFY cmdUriChanged)
    Q_PROPERTY(QString updateUri READ updateUri WRITE setUpdateUri NOTIFY updateUriChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int heartbeatPeriod READ heartbeatPeriod WRITE heartbeatPeriod NOTIFY heartbeatPeriodChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)
    Q_PROPERTY(State sState READ sState NOTIFY sStateChanged)
    Q_PROPERTY(State cState READ cState NOTIFY cStateChanged)
    Q_ENUMS(State)

public:
    explicit QComponent(QQuickItem *parent = 0);

    enum State {
        STATE_DISCOVER = 0,
        STATE_DOWN = 1,
        STATE_TRYING = 2,
        STATE_UP = 3
    };

    virtual void componentComplete();

    QString cmdUri() const
    {
        return m_cmdUri;
    }

    QString updateUri() const
    {
        return m_updateUri;
    }

    QString name() const
    {
        return m_name;
    }

    int heartbeatPeriod() const
    {
        return m_heartbeatPeriod;
    }

    bool isSynced() const
    {
        return m_synced;
    }

    State sState() const
    {
        return m_sState;
    }

    State cState() const
    {
        return m_cState;
    }

public slots:
    void connectToHost();
    void bind();
    void ready();
    void exit();

    void pinUpdate(pb::Pin remotePin, QPin *localPin);
    void pinChange(QVariant value);

    void setCmdUri(QString arg)
    {
        if (m_cmdUri != arg) {
            m_cmdUri = arg;
            emit cmdUriChanged(arg);
        }
    }

    void setUpdateUri(QString arg)
    {
        if (m_updateUri != arg) {
            m_updateUri = arg;
            emit updateUriChanged(arg);
        }
    }

    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void heartbeatPeriod(int arg)
    {
        if (m_heartbeatPeriod != arg) {
            m_heartbeatPeriod = arg;
            emit heartbeatPeriodChanged(arg);
        }
    }

private:
    QString m_cmdUri;
    QString m_updateUri;
    QString m_name;
    int m_heartbeatPeriod;
    bool m_synced;
    State m_sState;
    State m_cState;

    ZMQContext *m_context;
    ZMQSocket  *m_updateSocket;
    ZMQSocket  *m_cmdSocket;
    int         m_instanceCount;
    int         m_pingOutstanding;
    // more efficient to reuse a protobuf Message
    pb::Container m_rx;
    pb::Container m_tx;
    QMap<QString, QPin*> m_pinsByName;
    QHash<int, QPin*> m_pinsByHandle;
    QTimer      *m_heartbeatTimer;

    QObjectList recurseObjects(const QObjectList &list);



private slots:
    void updateMessageReceived(QList<QByteArray> messageList);
    void cmdMessageReceived(QList<QByteArray> messageList);
    void hearbeatTimerTick();

signals:
void cmdUriChanged(QString arg);
void updateUriChanged(QString arg);
void nameChanged(QString arg);
void heartbeatPeriodChanged(int arg);
void syncedChanged(bool arg);
void protocolError(QStringList notes);
void sStateChanged(State arg);
void cStateChanged(State arg);
};

#endif // QCOMPONENT_H
