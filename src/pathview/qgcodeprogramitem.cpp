#include "qgcodeprogramitem.h"

QGCodeProgramItem::QGCodeProgramItem(const QString &fileName, int lineNumber):
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_gcode(QString("")),
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



