#ifndef QGCODEPROGRAMMODEL_H
#define QGCODEPROGRAMMODEL_H

#include <QAbstractListModel>
#include "qgcodeprogramitem.h"

class QGCodeProgramModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(GCodeProgramRoles)

public:
    enum GCodeProgramRoles {
            FileNameRole = Qt::UserRole,
            LineNumberRole,
            GCodeRole,
            PreviewRole,
            SelectedRole,
            ActiveRole,
            ExecutedRole
        };

    explicit QGCodeProgramModel(QObject *parent = 0);
    ~QGCodeProgramModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(const QString &fileName, int lineNumber) const;
    QModelIndex index(int row);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void prepareFile(const QString &fileName, int lineCount);
    void removeFile(const QString &fileName);
    void addLine(const QString &fileName);
    QVariant data(const QString &fileName, int lineNumber, int role) const;
    bool setData(const QString &fileName, int lineNumber, const QVariant &value, int role);
    void clear();
    void beginUpdate();
    void endUpdate();

private:
    typedef struct {
        int index;
        int count;
    } FileIndex;

    QList<QGCodeProgramItem*> m_items;
    QHash<QString, FileIndex> m_fileIndices;

    QVariant internalData(const QModelIndex &index, int role) const;
    bool internalSetData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // QGCODEPROGRAMMODEL_H
