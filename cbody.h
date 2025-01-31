#ifndef CBODY_H
#define CBODY_H

#include <QVector>

#include "cconnection.h"

// Класс тела, состоит из точек масс и связей между точками

class CBody
{
public:
    typedef QVector<CConnection> QConnections;
//    typedef QVector<CMassPoint> QMassPoints;
    typedef QList<CMassPoint> QMassPoints;

public:
    CBody();
    CBody(const QConnections &connections, const QMassPoints &massPoints) : m_vConnections(connections), m_vMassPoints(massPoints) {}

    void addConnection(CMassPoint *first, CMassPoint *second, qreal length = 0)
        { m_vConnections << CConnection(first, second, length); }

    const QConnections& connections() const
        { return m_vConnections; }

    QConnections& connections()
        { return m_vConnections; }

    QMassPoints& massPoints()
        { return m_vMassPoints; }

    const QMassPoints& massPoints() const
        { return m_vMassPoints; }

    qreal mass() const
        { return m_pointMass; }

    qreal rigity() const
        { return m_connectionRigity; }

    void solve(qreal dt, int iterCount, const QVector<QVector2D> &forces);

    bool hitTest(const QPointF& worldPos, CMassPoint **hitPoint) const;

    void moveTo(QPointF pos, const CMassPoint *origin);

public slots:
    void setMass(const qreal &pointMass)
        { m_pointMass = pointMass; }

    void setRigity(const qreal &connectionRigity)
        { m_connectionRigity = connectionRigity; }

    void deleteMassPoint(const CMassPoint &point);

protected:
    void solvePoints(qreal dt, const QVector<QVector2D> &forces);
    void solveConnections();
    void fixPinnedPoints();

private:
    QConnections    m_vConnections;         // Вектор соединений
    QMassPoints     m_vMassPoints;          // Вектор точек масс
    qreal           m_pointMass = 1;        // Масса точек
    qreal           m_connectionRigity = 0.7;   // Жесткость соединений
};

#endif // CBODY_H
