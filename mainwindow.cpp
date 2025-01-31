#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(m_dtLabel = new QLabel);
    QMap <QString, CBody>::const_iterator i = world()->bodies().constBegin();
      while (i != world()->bodies().constEnd()) {
        ui->bodiesCombo->addItem(i.key());
        ++i;
      }
    clearCombo();

    ui->GValueSpinBox->setValue(world()->gravity().length());
    ui->WValueSpinBox->setValue(world()->wind().length());
    QVector2D wind = world()->wind();
    QLineF windLine(QPointF(), wind.toPointF());
    ui->WAngleSpinBox->setValue(windLine.angle());
    QVector2D gravity = world()->gravity();
    QLineF gravityLine(QPointF(), gravity.toPointF());
    ui->GAngleSpinBox->setValue(gravityLine.angle());

    connect(ui->widget, SIGNAL(bodyWasSelected(const CBody*)), this, SLOT(onBodyWasSelected(const CBody*)));
    connect(ui->playButton, SIGNAL(toggled(bool)), this, SLOT(onPlayToggled(bool)));
    connect(ui->resetButton, SIGNAL(clicked()), world(), SLOT(reset()));
    connect(ui->itCountSpinBox, SIGNAL(valueChanged(int)), world(), SLOT(setIterCount(int)));
    connect(ui->massSpinBox, SIGNAL(valueChanged(double)), ui->widget, SLOT(onMassWasChanged(qreal)));
    connect(ui->rigitySpinBox, SIGNAL(valueChanged(double)), ui->widget, SLOT(onRigityWasChanged(qreal)));
    connect(world(), SIGNAL(updated(int)), this, SLOT(onWorldUpdated(int)));
    connect(ui->WAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateWind()));
    connect(ui->GAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateGravity()));
    connect(ui->WValueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateWind()));
    connect(ui->GValueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateGravity()));
    connect(ui->WEnableCheck, SIGNAL(stateChanged(int)), world(), SLOT(enableWind(int)));
    connect(ui->GEnableCheck, SIGNAL(stateChanged(int)), world(), SLOT(enableGravity(int)));
    connect(ui->bodiesCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(comboChanged(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onWorldUpdated(int msecTaken)
{
    if(m_dtLabel)
        m_dtLabel->setText(QString("%1 msec").arg(msecTaken));
}

void MainWindow::onBodyWasSelected(const CBody* pBody)
{
    if(pBody)
    {
        ui->massSpinBox->setValue(pBody->mass());
        ui->rigitySpinBox->setValue(pBody->rigity());
    }
    ui->massSpinBox->setEnabled(pBody != nullptr);
    ui->rigitySpinBox->setEnabled(pBody != nullptr);
}

void MainWindow::onPlayToggled(bool bPlay)
{
    world()->setPlay(bPlay);
}

void MainWindow::updateWind()
{
    double angle = ui->WAngleSpinBox->value();
    double value = ui->WValueSpinBox->value();
    world()->setWind(angle, value);
    ui->widget->update();
}

void MainWindow::updateGravity()
{
    double angle = ui->GAngleSpinBox->value();
    double value = ui->GValueSpinBox->value();
    world()->setGravity(angle, value);
    ui->widget->update();
}

void MainWindow::comboChanged(QString key)
{
    if(world()->bodies().contains(key))
    {
        ui->widget->selectBody(&world()->bodies()[key]);
        ui->widget->update();
        onBodyWasSelected(&world()->bodies()[key]);
    }

}

void MainWindow::clearCombo()
{
    ui->bodiesCombo->setCurrentIndex(-1);
}

