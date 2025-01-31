#include "qworldwidget.h"

#include <QMapIterator>
#include <QPainter>
#include <QDebug>
#include "qworld.h"
#include "qsettings.h"

QWorldWidget::QWorldWidget(QWidget *parent) : QWidget(parent)
{
    connect(world(), SIGNAL(needUpdate()), this, SLOT(onWorldNeedUpdate()));
}

void QWorldWidget::onMassWasChanged(qreal mass)
{
    selectedBody()->setMass(mass);
    update();
    qDebug() << selectedBody()->mass();
}

void QWorldWidget::onRigityWasChanged(qreal rigity)
{
    selectedBody()->setRigity(rigity);
    update();
    qDebug() << selectedBody()->rigity();
}

void QWorldWidget::paintBody(QPainter &painter, const CBody &body)
{
    QColor bodyColor = Qt::white;
    if(m_selectedBody == &body)
        bodyColor = settings()->selectedBodyColor()/*Qt::cyan*/;
    painter.setPen(bodyColor);

    for(int i = 0; i < body.connections().size(); ++i)
        painter.drawLine(pointToWidget(body.connections().at(i).firstPoint()->pos()),
                         pointToWidget(body.connections().at(i).secondPoint()->pos()));

    QRectF rcPoint(0, 0, 3, 3);
    for(int i = 0; i < body.massPoints().size(); ++i)
    {
        if (m_selectedPoint == &(body.massPoints().at(i)))
            painter.setPen(Qt::red);
        else
            painter.setPen(bodyColor);
        rcPoint.moveCenter(pointToWidget(body.massPoints().at(i).pos()));
        painter.drawEllipse(rcPoint);
        if (body.massPoints().at(i).isPinned())
        {
            painter.drawLine(rcPoint.topLeft(), rcPoint.bottomRight());
            painter.drawLine(rcPoint.topRight(), rcPoint.bottomLeft());
        }
    }
}

bool QWorldWidget::hitTest(const QPointF &pos, CBody **hitBody, CMassPoint **hitPoint) const
{
    QPointF worldPos = pointToWorld(pos - rect().center());
    *hitBody = nullptr;
    foreach(const CBody& body, world()->bodies())
        if (body.hitTest(worldPos, hitPoint))
        {
            *hitBody = const_cast <CBody*>(&body);
            return true;
        }
    return false;
}

void QWorldWidget::setOrigin(QPointF point)
{
    m_origin = point;
    update();
}

void QWorldWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);
    painter.translate(rect().center());

    painter.setPen(Qt::white);

    foreach(const CBody& body, world()->bodies())
        paintBody(painter, body);
}

void QWorldWidget::mousePressEvent(QMouseEvent *event)
{
    hitTest(event->pos(), &m_selectedBody, &m_selectedPoint);

    if(event->button() == Qt::LeftButton)
    {
        m_dragStartPos = event->pos();
        m_originStartPos = m_origin;
        if(m_selectedPoint && world()->isPlay())
            m_selectedPoint->setPin(pointToWorld(event->pos() - rect().center()));
        emit bodyWasSelected(m_selectedBody);
        update();                       // Обновить выделенные
    }
    if(event->button() == Qt::RightButton)
        if(m_selectedPoint)
            m_selectedBody->deleteMassPoint(*m_selectedPoint);
}

void QWorldWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons() & Qt::LeftButton)
    {
        if(m_selectedBody == nullptr && m_selectedPoint == nullptr)
            setOrigin(m_originStartPos + (m_dragStartPos - event->pos()) / m_scale);    // Перемещение центра мира
        else
        {
            if(world()->isPlay())
            {
                m_selectedPoint->setPin(pointToWorld(event->pos() - rect().center()));// Приколем точку
            }
            else
                // Передвинем тело
                m_selectedBody->moveTo(pointToWorld(event->pos() - rect().center()), m_selectedPoint);
            update();
        }
    }
}

void QWorldWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_selectedPoint && world()->isPlay())
        m_selectedPoint->resetPin();
}

void QWorldWidget::wheelEvent(QWheelEvent *event)
{
    // Изменение масштаба
       qreal scaleFactor = 1.5;
       // Изменение масштаба мб линейным, как у тебя,
       // а мб таким, так мы никогда не сделаем масштаб <=0
       qreal scaleRatio = 1;
       if(event->angleDelta().y() < 0)
           scaleRatio = 1 / scaleFactor;
       if(event->angleDelta().y() > 0)
           scaleRatio = scaleFactor;

       // Мировые координаты мыши
       QPointF worldMousePos = pointToWorld(event->posF() - rect().center());
       // Расстояние от мыши до предыдущего центра в мировых координатах
       QPointF delta = worldMousePos - m_origin;
       // Предыдущий масштаб
       qreal prevScale = m_scale;

       m_scale *= scaleRatio;

       // Расстояние от мыши до нового центра в мировых координатах
       QPointF newDelta = delta * prevScale / m_scale;

       // Откладываем это расстояние от мыши
       QPointF newOrigin = worldMousePos - newDelta;
       m_origin = newOrigin;


       update();


    event->accept();
}
