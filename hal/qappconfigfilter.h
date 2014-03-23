#ifndef QAPPCONFIGFILTER_H
#define QAPPCONFIGFILTER_H

#include <QQuickItem>
#include "qappconfigitem.h"

class QAppConfigFilter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QAppConfigItem::ApplicationType type READ type WRITE setType NOTIFY typeChanged)


public:
    explicit QAppConfigFilter(QQuickItem *parent = 0);

    QAppConfigItem::ApplicationType type() const
    {
        return m_type;
    }

public slots:

    void setType(QAppConfigItem::ApplicationType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }

private:
    QAppConfigItem::ApplicationType m_type;

signals:

    void typeChanged(QAppConfigItem::ApplicationType arg);
};

#endif // QAPPCONFIGFILTER_H
