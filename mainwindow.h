#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

#include "cbody.h"

// Класс основного окна, отвечает за взаимодействие с инпутами окна

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onWorldUpdated(int msecTaken);
    void onBodyWasSelected(const CBody*);
    void onPlayToggled(bool bPlay);
    void updateWind();
    void updateGravity();
    void comboChanged(QString key);
    void clearCombo();

protected:


private:
    Ui::MainWindow *ui;

    QLabel *m_dtLabel = nullptr;

};

#endif // MAINWINDOW_H
