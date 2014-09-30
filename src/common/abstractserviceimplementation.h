#ifndef ABSTRACTSERVICEIMPLEMENTATION_H
#define ABSTRACTSERVICEIMPLEMENTATION_H

#include <QObject>
#include <QQmlParserStatus>

class AbstractServiceImplementation : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(bool required READ required WRITE setRequired NOTIFY requiredChanged)
    //Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    //Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    //Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    //Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit AbstractServiceImplementation(QObject *parent = 0);

    /*enum State {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Error = 3
    };

    enum ConnectionError {
        NoError = 0,
        ServiceError = 1,
        TimeoutError = 2,
        SocketError = 3
    };*/

    void classBegin() {}
    void componentComplete();

    bool isComponentCompleted() {
        return m_componentCompleted;
    }

    bool required() const
    {
        return m_required;
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setRequired(bool arg)
    {
        if (m_required == arg)
            return;

        m_required = arg;
        emit requiredChanged(arg);
    }

    void setReady(bool arg);

private:
    bool m_componentCompleted;
    bool m_required;
    bool m_ready;

    virtual void start() = 0;
    virtual void stop() = 0;

private slots:

signals:
    void requiredChanged(bool arg);
    void readyChanged(bool arg);
};

#endif // ABSTRACTSERVICEIMPLEMENTATION_H
