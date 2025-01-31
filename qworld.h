#ifndef QWORLD_H
#define QWORLD_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QDebug>
#include "cbody.h"

// Класс мира, отвечает за рассчеты и симуляцию, хранит в себе список тел

class QWorld : public QObject
{
    Q_OBJECT

public:
    typedef QMap<QString, CBody> QBodies;

public:
    explicit QWorld(QObject *parent = nullptr);

    const QBodies& bodies() const      // Возвращает словарь с телами
        { return m_bodies; }

    QBodies& bodies()                  // Возвращает словарь с телами
        { return m_bodies; }

    // Добавление тела
    void addBody(const QString &key, const CBody &body);

    void createBodies();

    bool isPlay() const
        { return m_bPlay; }

    int iterCount() const
        { return m_iterCount; }

    QVector2D wind() const
        { return m_wind; }

    QVector2D gravity() const
        { return m_gravity; }

signals:
    void needUpdate();
    void updated(int msecTaken);

public slots:
    void solve(qreal dT, const QVector<QVector2D> &forces);
    void start()
        { m_bPlay = true; }
    void stop()
        { m_bPlay = false; }
    void setPlay(bool play);
    void reset();
    void setIterCount(int value)
        { m_iterCount = value; }

    void setWind(double angle, double val);
    void enableWind(int enable)
        { m_ifWindEnabled = enable; }
    void setGravity(double angle, double val);
    void enableGravity(int enable)
        { m_ifGravityEnabled = enable; }

protected:
    void createCloth(int rows, int columns, qreal cellSize, QPointF start);
    void createRope(int segments, const QPointF& segmentOffset, const QPointF& start, const QString &id);
    void createCube(int size, const QPointF& start, const QString& id);

private:
    QBodies m_bodies;
    QBodies m_bodiesBackup;

    int m_iterCount = 5;

    bool m_bPlay = false;
    qreal m_tempo = 1;
    int m_iPrevTick = -1;

    QVector2D m_wind = {1, 0};
    bool m_ifWindEnabled = false;
    QVector2D m_gravity = {0, 9.8};
    bool m_ifGravityEnabled = false;

protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};
QWorld *world();
#endif // QWORLD_H
