#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
//#include <QStandardPaths>
#include <QPixmap>
#include <QMessageBox>
#include <QDesktopServices>
//#include <QtPlugin>
#include <QTimer>
#include <QDebug>

//Q_IMPORT_PLUGIN(qsqloci)
//Q_IMPORT_PLUGIN(qgif)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void resizeEvent(QResizeEvent* event);
    ~MainWindow();

private slots:
    void on_actionOtw_rz_triggered();

    void on_actionKoniec_triggered();

    //void on_MainWindow_iconSizeChanged(const QSize &iconSize);

    void on_actionPraca_krokowa_triggered();

    void on_actionRozwi_ca_e_zadanie_triggered();

    void on_actionPrzeszukiwanie_wszerz_triggered();

    void on_actionPrzeszukiwanie_wg_b_triggered();

    void StepTimerTimeout();

private:
    Ui::MainWindow *ui;
    QLabel *label;
    QPixmap *pixmap;
    bool kindOfJob;
    QString filePath;
    QTimer *StepTimer;
};

#endif // MAINWINDOW_H
