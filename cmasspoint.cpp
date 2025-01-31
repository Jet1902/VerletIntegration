#include "cmasspoint.h"

#include <QVector>

CMassPoint::CMassPoint()
{

}

void CMassPoint::move(QPointF offset)
{
    m_pos += offset;
    m_prevPos += offset;
    m_pinPos += offset;
}

void CMassPoint::solve(qreal dt, qreal mass, const QVector <QVector2D> &forces)
{
    QVector2D forcesSumm(0,0);
    foreach(QVector2D force, forces)
        forcesSumm += force;
    // Первым идет гравитация
    QPointF newPos = 2 * m_pos - m_prevPos + (forcesSumm * dt * dt / 1000000).toPointF();
    // Вторым идет ветер
//    newPos += (forces.at(1)/1000).toPointF(); // коэффициент влияния ветра - 1000
    m_prevPos = m_pos;
    m_pos = newPos;
}

void CMassPoint::fixPinned()
{
    if(m_anchor != Hard)
        return;
    m_pos = m_pinPos;
}












