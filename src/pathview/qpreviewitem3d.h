#ifndef QPREVIEWITEM3D_H
#define QPREVIEWITEM3D_H

#include <QVector3D>

class QPreviewItem3D
{
public:
    enum Type {
        Line,
        Arc
    };

    QPreviewItem3D();

private:
    QVector3D m_position;
    QVector3D m_vector;
};

#endif // QPREVIEWITEM3D_H
