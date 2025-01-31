#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QColor>
#include <QObject>


class QSettings: public QObject
{
    Q_OBJECT

public:

    explicit QSettings(QObject *parent = nullptr);

    QColor selectedBodyColor() const
        { return m_selectedBodyColor; }

signals:
    void selectedBodyColorChanged();

public slots:

    void setSelectedBodyColor(const QColor &value)
        { m_selectedBodyColor = value; }

private:
    QColor m_selectedBodyColor = Qt::cyan;
};
QSettings *settings();
#endif // CSETTINGS_H
