#include "cconnection.h"

void CConnection::solve(qreal stiffness)
{
    QVector2D delta(m_secondPoint->pos() - m_firstPoint->pos());
    qreal curLength = delta.length();
    delta.normalize();

    qreal diff = - restLength + curLength;

    m_firstPoint->setPos(m_firstPoint->pos() + (delta * diff * stiffness).toPointF());
    m_secondPoint->setPos(m_secondPoint->pos() - (delta * diff * stiffness).toPointF());
}
