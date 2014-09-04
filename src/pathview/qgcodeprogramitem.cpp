#include "qgcodeprogramitem.h"

QGCodeProgramItem::QGCodeProgramItem(const QString &fileName, int lineNumber):
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_gcode(QString("")),
    m_selected(false),
    m_active(false),
    m_executed(false),
    m_previewList(NULL)
{
}

QGCodeProgramItem::~QGCodeProgramItem()
{
}

QString QGCodeProgramItem::fileName() const
{
    return m_fileName;
}

void QGCodeProgramItem::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}
int QGCodeProgramItem::lineNumber() const
{
    return m_lineNumber;
}

void QGCodeProgramItem::setLineNumber(int lineNumber)
{
    m_lineNumber = lineNumber;
}
QString QGCodeProgramItem::gcode() const
{
    return m_gcode;
}

void QGCodeProgramItem::setGcode(const QString &gcode)
{
    m_gcode = gcode;
}
QList<pb::Preview> *QGCodeProgramItem::previewList() const
{
    return m_previewList;
}

void QGCodeProgramItem::setPreviewList(QList<pb::Preview> *previewList)
{
    m_previewList = previewList;
}
bool QGCodeProgramItem::selected() const
{
    return m_selected;
}

void QGCodeProgramItem::setSelected(bool selected)
{
    m_selected = selected;
}
bool QGCodeProgramItem::active() const
{
    return m_active;
}

void QGCodeProgramItem::setActive(bool active)
{
    m_active = active;
}
bool QGCodeProgramItem::executed() const
{
    return m_executed;
}

void QGCodeProgramItem::setExecuted(bool executed)
{
    m_executed = executed;
}






