#include "cbody.h"

#include <QRectF>
#include <QDebug>
#include <QVector>

CBody::CBody()
{

}

void CBody::solve(qreal dt, int iterCount, const QVector <QVector2D> &forces)
{
    solvePoints(dt, forces);
    for( int i = 0; i < iterCount; ++i )
    {
        solveConnections();
        fixPinnedPoints();
    }
}

bool CBody::hitTest(const QPointF &worldPos, CMassPoint **hitPoint) const
{
    *hitPoint = nullptr;

    QRectF rcPoint(0, 0, 10, 10);
    rcPoint.moveCenter(worldPos);
    qreal distToWorldPos = 1000000000;

    foreach(const CMassPoint& point, massPoints())
        if(rcPoint.contains(point.pos()))
        {
            qreal currentDist = (worldPos.x() - point.pos().x()) * (worldPos.x() - point.pos().x()) + (worldPos.y() - point.pos().y()) * (worldPos.y() - point.pos().y());
            if( currentDist < distToWorldPos )
            {
                distToWorldPos = currentDist;
                *hitPoint = const_cast <CMassPoint*> (&point);
            }
        }
    return (*hitPoint != nullptr);
}

void CBody::moveTo(QPointF pos, const CMassPoint *origin)
{
    QPointF offset;
    foreach(const CMassPoint& point, m_vMassPoints)
        if(&point == origin)
        {
            offset = pos - point.pos();
            break;
        }

    for(QMassPoints::iterator it = m_vMassPoints.begin(); it != m_vMassPoints.end(); ++it)
    {
        it->move(offset);
    }
}

void CBody::solvePoints(qreal dt, const QVector <QVector2D> &forces)                                                                          // Для каждой точки вызывать solve
{
    for(QMassPoints::iterator it = m_vMassPoints.begin(); it != m_vMassPoints.end(); ++it)
        it->solve(dt, m_pointMass, forces);
}

void CBody::solveConnections()
{
    for(QConnections::iterator it = m_vConnections.begin(); it != m_vConnections.end(); ++it)
        it->solve(m_connectionRigity);
}

void CBody::fixPinnedPoints()
{
    for(QMassPoints::iterator it = m_vMassPoints.begin(); it != m_vMassPoints.end(); ++it)
        it->fixPinned();
}

void CBody::deleteMassPoint(const CMassPoint &point)
{
    for(int i = 0; i < connections().size(); ++i)
        if(connections().at(i).firstPoint() == &point || connections().at(i).secondPoint() == &point)
            connections().remove(i--);
    for(int i = 0; i < massPoints().size(); ++i)
        if(&massPoints().at(i) == &point)
            massPoints().removeAt(i);

}












