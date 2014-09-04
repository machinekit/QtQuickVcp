#ifndef QGCODEPROGRAMITEM_H
#define QGCODEPROGRAMITEM_H
#include <QVariant>
#include <QList>
#include "preview.pb.h"

class QGCodeProgramItem
{
public:
    explicit QGCodeProgramItem(const QString &fileName, int lineNumber);
    ~QGCodeProgramItem();

    QString fileName() const;
    void setFileName(const QString &fileName);

    int lineNumber() const;
    void setLineNumber(int lineNumber);

    QString gcode() const;
    void setGcode(const QString &gcode);

    QList<pb::Preview> *previewList() const;
    void setPreviewList(QList<pb::Preview> *previewList);

    bool selected() const;
    void setSelected(bool selected);

    bool active() const;
    void setActive(bool active);

    bool executed() const;
    void setExecuted(bool executed);

private:
    QString m_fileName;
    int m_lineNumber;
    QString m_gcode;
    bool m_selected;
    bool m_active;
    bool m_executed;
    QList<pb::Preview> *m_previewList;
};

#endif // QGCODEPROGRAMITEM_H
