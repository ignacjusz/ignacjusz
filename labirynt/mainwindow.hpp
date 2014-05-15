#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "alg.hpp"
#include <QMainWindow>
#include <QPainter>
#include <QRect>
#include <QMouseEvent>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_zoomIn_clicked();

	void on_zoomOut_clicked();

	void on_openImage_clicked();

	void on_zoomNo_clicked();

	void on_symSpeedVal_valueChanged(int value);

	void on_symColorVal_valueChanged(int value);

	void on_startStop_clicked();

	void on_symColorVisited_stateChanged(int arg1);

	void on_saveImage_clicked();

public slots:
	void nextStep();

private:
	Ui::MainWindow *ui;
	const QPoint paintAreaCorner;
	QRect paintArea;
	double zoom;
	QPoint imageCenter;
	//QPoint paintCenter;
	QPoint startDrag;
	QPoint startPaint;
	QString imageName;
	QString statsName;
	QFile statsFile;
	QString windowTitle;
	QTextStream * statsStream;

	QTimer * timer;

	double realZoom( double zoom );
	double reverseZoom( double realZoom );

	Alg alg;

	void startStopPoints( QPoint start, QPoint stop );

	double stepTimeMsec;
	double stepMul;

	void paintEvent( QPaintEvent *e );
	void mousePressEvent( QMouseEvent *e );
	void mouseReleaseEvent( QMouseEvent *e );
	void wheelEvent( QWheelEvent *e );
	void resizeEvent( QResizeEvent *e);
};

#endif // MAINWINDOW_HPP
