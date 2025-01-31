#include "qworld.h"

#include <QTime>

static QWorld* g_pWorld = nullptr;

QWorld* world()
{
    if(!g_pWorld)
        g_pWorld = new QWorld;
    return g_pWorld;
}

QWorld::QWorld(QObject *parent) : QObject(parent)
{
    createBodies();
    startTimer(0);
}

void QWorld::addBody(const QString &key, const CBody &body)
{
    m_bodies[key] = body;
}

void QWorld::createBodies()
{
    createCloth(50, 50, 3, QPointF(-100, -100));
//    QString id1 = "Rope";
//    QString id2 = "Cube";
//    QString id3 = "Lone Rope";
//    createRope(100, QPointF(1, -1), QPointF(-100, -100), id1);
//    createCube(200, QPointF(-100, -100), id2);
//    createRope(100, QPointF(1, -1), QPointF(100, -100), id3);
//    if(m_bodies.contains(id3))
//        m_bodies[id3].massPoints().last().setPin(m_bodies[id3].massPoints().last().pos());
//    if(m_bodies.contains("Rope"))
//        m_bodies["Rope"].massPoints().last().setPin(m_bodies["Rope"].massPoints().last().pos());
//    if(m_bodies.contains("Cube"))
//    {
//        m_bodies["Cube"].addConnection(&m_bodies["Rope"].massPoints().first(), &m_bodies["Cube"].massPoints().first());
//        m_bodies["Cube"].addConnection(&m_bodies[id3].massPoints().first(), &m_bodies["Cube"].massPoints()[1]);
//        m_bodies["Cube"].setRigity(0.9);
//    }

}


void QWorld::solve(qreal dT, const QVector<QVector2D> &forces)
{
    int start = QTime::currentTime().msecsSinceStartOfDay();
    for(QBodies::iterator it = m_bodies.begin(); it != m_bodies.end(); ++it)
        it->solve(dT, m_iterCount, forces);
    emit needUpdate();

    int finish = QTime::currentTime().msecsSinceStartOfDay();
    emit updated(finish - start);
}

void QWorld::setPlay(bool play)
{
    m_bPlay = play;
    emit needUpdate();
}

void QWorld::reset()
{
    m_bodies.clear();
    createBodies();
}

void QWorld::setWind(double angle, double val)
{
    QLineF line(QPointF(), m_wind.toPointF());
    if(line.isNull())
        line.setP2(line.p2() + QPointF(1, 1));
    line.setLength(val);
    line.setAngle(angle);
    m_wind = QVector2D(line.p2());
}

void QWorld::setGravity(double angle, double val)
{
    QLineF line(QPointF(), m_gravity.toPointF());
    if(line.isNull())
        line.setP2(line.p2() + QPointF(1, 1));
    line.setLength(val);
    line.setAngle(angle);
    m_gravity = QVector2D(line.p2());
}

void QWorld::createCloth(int rows, int columns, qreal cellSize, QPointF start)
{
    QPointF pos = start;
    CBody::QMassPoints points;
//    points.reserve(rows * columns);
    CBody::QConnections connections;
    for(int column = 0; column < columns; ++column)
    {
        pos.rx() = start.x() + column * cellSize;
        pos.ry() = start.y();
        for(int row = 0; row < rows; ++row)
        {
            pos.ry() = start.y() + row * cellSize;
            points << CMassPoint(pos);

            if(column)
                connections << CConnection(&points.last(), &points[points.size() - rows - 1]);
//            if(column && row != rows - 1)
//                connections << CConnection(&points.last(), &points[points.size() - rows]);
//            if(column && row)
//                connections << CConnection(&points.last(), &points[points.size() - rows - 2]);
            if(row)
                connections << CConnection(&points.last(), &points[points.size() - 2]);
            else /*if(column == 0 || column == columns - 1)*/
                points.last().setPin(pos, CMassPoint::Hard);
        }
    }
    addBody("Cloth", CBody(connections, points));

}

void QWorld::createRope(int segments, const QPointF &segmentOffset, const QPointF &start, const QString& id)
{
    QPointF pos = start;
    CBody::QMassPoints points;
    CBody::QConnections connections;

    for(int i = 0; i < segments + 1; ++i)
    {
        points << CMassPoint(pos);
        pos += segmentOffset;

//        if(i)
            connections << CConnection(&points.last(), &points[points.size() - 2]);
    }
    addBody(id, CBody(connections, points));
}

void QWorld::createCube(int size, const QPointF &start, const QString& id)
{
    QPointF pos = start;
    CBody::QMassPoints points;
    CBody::QConnections connections;

    points << CMassPoint(pos);
    points << CMassPoint(pos + QPointF(size, 0));
    points << CMassPoint(pos + QPointF(size, size));
    points << CMassPoint(pos + QPointF(0, size));

    for(int i = 0; i < 3; ++i)
        connections << CConnection(&points[i], &points[i + 1]);

    connections << CConnection(&points[3], &points[0]);
    connections << CConnection(&points[2], &points[0]);
    connections << CConnection(&points[1], &points[3]);

    addBody(id, CBody(connections, points));
}


void QWorld::timerEvent(QTimerEvent* /*event*/)
{
    if(m_bPlay)
    {
        int curTick = QTime::currentTime().msecsSinceStartOfDay();
        if(m_iPrevTick != -1)
        {
            qreal dT = (curTick - m_iPrevTick) * m_tempo;
            if(dT < 5)
                return;
            QVector<QVector2D> forces;
            if(m_ifWindEnabled)
                forces << m_wind;
            if(m_ifGravityEnabled)
                forces << m_gravity;
//            forces << m_gravity << m_wind;
            solve(dT, forces);
        }
        m_iPrevTick = curTick;
    }
    else
        m_iPrevTick = -1;
}
