#ifndef CMASSPOINT_H
#define CMASSPOINT_H

#include <QPointF>
#include <QVector2D>

// Класс точки массы

class CMassPoint
{
public:
    enum PinRigity{ None = 0, Light = 1, Hard = 2 };            // Степень жесткости закрепления точек масс

    CMassPoint();
    CMassPoint(const QPointF &pos) : m_pos(pos), m_prevPos(pos) {}

    QPointF pos() const
        { return m_pos; }
    void setPos(const QPointF &pos)
        { m_pos = pos; }

    PinRigity rigity() const
        { return m_anchor; }

    QPointF pinPos() const
        { return m_pinPos; }

    bool isPinned() const
    { return m_anchor != None; }

    void setPin(const QPointF &pinPos, const PinRigity &rigity = Hard)
    {
        m_pinPos = pinPos;
        m_anchor = rigity;
    }

    void resetPin()
    {
        m_anchor = None;
    }

    void move(QPointF offset);

    QVector2D acceleration() const
        { return m_acceleration; }

    void resetAcceleration()
        { m_acceleration = QVector2D(); }
    void addAcceleration(QVector2D acc)
        { m_acceleration += acc; }

    void solve(qreal dt, qreal mass, const QVector<QVector2D> &forces);
    void fixPinned();

private:
    QPointF m_pos;                                  // Координата в мировом пространстве
    QPointF m_prevPos;                              // Предыдущее положение точки в мировом пространстве
    PinRigity m_anchor = None;                      // Жесткость якоря
    QPointF m_pinPos;                               // Позиция якоря
    QVector2D m_acceleration;
};

#endif // CMASSPOINT_H
