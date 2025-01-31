#ifndef CCONNECTION_H
#define CCONNECTION_H

#include "cmasspoint.h"

#include <QLine>
#include <QtMath>

// Класс связи между точками масс

class CConnection
{
public:
    CConnection(){}
    // Если длина при конструировании равна 0, длина равна расстоянию между точками
    CConnection(CMassPoint *first, CMassPoint *second, qreal length = 0) : m_firstPoint(first), m_secondPoint(second), restLength(length)
    {
        if(restLength == 0.)
        {
            QPointF delta = firstPoint()->pos() - secondPoint()->pos();
            restLength = qSqrt(delta.x() * delta.x() + delta.y() * delta.y());
        }

    }

    const CMassPoint *firstPoint() const
        { return m_firstPoint; }

    const CMassPoint *secondPoint() const
        { return m_secondPoint; }

    qreal getRestLength() const
        { return restLength; }

    void solve(qreal stiffness);

private:
    CMassPoint *m_firstPoint = nullptr;                         // Указатель на первую точку
    CMassPoint *m_secondPoint = nullptr;                        // Указатель на вторую точку
    qreal restLength = 0;                                       // Длина в покое
};

#endif // CCONNECTION_H
