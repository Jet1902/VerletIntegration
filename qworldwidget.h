#ifndef QWORLDWIDGET_H
#define QWORLDWIDGET_H

#include <QWidget>
#include "qworld.h"
#include <QWheelEvent>

// Класс виджета, отвечает за отрисовку мира и взаимодействие с ним

class QWorldWidget : public QWidget
{
    Q_OBJECT

public:

    QWorldWidget(QWidget *parent = nullptr);

    qreal scaledToWidget(qreal dist) const
        { return dist * m_scale; }

    QPointF pointToWidget(const QPointF &point) const
        { return (point - m_origin) * m_scale; }
    QPointF pointToWorld(const QPointF &point) const
        { return point / m_scale + m_origin; }


    CBody *selectedBody() const
        { return m_selectedBody; }
    void selectBody(CBody *selectedBody)
        { m_selectedBody = selectedBody; }

    CMassPoint *selectedPoint() const
        { return m_selectedPoint; }
    void selectPoint(CMassPoint *selectedPoint)
        { m_selectedPoint = selectedPoint; }

public slots:

    void onMassWasChanged(qreal mass);
    void onRigityWasChanged(qreal rigity);

protected:
    void paintBody(QPainter& painter, const CBody &body);                   // Отрисовка тела
    bool hitTest(const QPointF& pos, CBody **hitBody, CMassPoint **hitPoint) const;

private:
    QPointF m_origin;                                                       // Положение центра виджета в мировых координатах
    qreal m_scale = 1;                                                      // Масштаб отображения (При 2 - вью в два раза шире)

//    bool m_drag = false;                                                    // Активно ли передвижение мира

    QPointF m_dragStartPos;                                                 // Начальная позиция курсора при передвижении мира
    QPointF m_originStartPos;                                               // Позиция начального центра при передвижении мира

    CBody* m_selectedBody;
    CMassPoint* m_selectedPoint;

signals:
    void bodyWasSelected(const CBody* body);

public slots:
    void setOrigin(QPointF point);                                          // Установка центра
    void onWorldNeedUpdate()
        { update(); }

protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
};

#endif // QWORLDWIDGET_H
