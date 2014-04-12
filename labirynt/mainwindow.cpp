#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cmath>
#define DE qDebug()

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
	paintAreaCorner( 210, 10), paintArea( paintAreaCorner.x(), paintAreaCorner.y(), 180, 180 ), zoom(1), imageCenter( 0, 0 ) {

	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::paintEvent(QPaintEvent * e) {
	QPainter painter(this);

	painter.drawRect( paintArea );
	if( srcImage.isNull() ) {
		return;
	}
	QImage printImage( srcImage );
	//paint queues

	int x=paintArea.width()/realZoom( zoom );
	int y=paintArea.height()/realZoom( zoom );

	QRect cutRect( imageCenter.x()-x/2, imageCenter.y()-y/2, x, y );

	painter.drawImage( paintArea, printImage, cutRect );

	//DE << "paintarea=" << paintArea << "imgctr=" << imageCenter << "cutrect=" << cutRect;
}

double MainWindow::realZoom(double zoom) {
	return std::pow( 10, zoom/10 );
}

double MainWindow::reverseZoom(double realZoom) {
	return ( std::log10( realZoom ) ) * 10;
}

void MainWindow::mousePressEvent(QMouseEvent * e) {
	if( paintArea.contains( e->pos() ) ) {
		if( e->button() == Qt::LeftButton ) {
			startDrag=e->pos();
		} else if( e->button() == Qt::RightButton ) {
			startPaint=e->pos();
		}
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent * e) {
	if( paintArea.contains( e->pos() ) ) {
		if( e->button() == Qt::LeftButton && paintArea.contains( startDrag ) ) {
			imageCenter+= (startDrag-e->pos()) / realZoom( zoom );
		} else if( e->button() == Qt::RightButton && paintArea.contains( startPaint ) ) {

			QRect paint( e->pos(), startPaint );

			paint=paint.normalized();
			DE << "paint=" << paint;

			int x=paintArea.width()/realZoom( zoom );
			int y=paintArea.height()/realZoom( zoom );

			QPoint mv( imageCenter.x()-x/2, imageCenter.y()-y/2 );

			paint.moveTo( ( paint.topLeft()-paintAreaCorner )/realZoom( zoom )+mv );

			paint.setWidth( paint.width()/realZoom( zoom ) );
			paint.setHeight( paint.height()/realZoom( zoom ) );

			DE << "transform=" << paint;

			QPainter p( & srcImage);

			p.setBrush( QColor( 255, 0, 0 ) );
			p.drawRect( paint );
			//TODO: here

		}
		repaint();
	}

}

void MainWindow::wheelEvent(QWheelEvent * e) {
	if( paintArea.contains( e->pos() ) ) {
		if( e->delta() > 0 ) {
			++zoom;
		} else {
			--zoom;
		}
		repaint();
	} else {
		e->ignore();
	}
}

void MainWindow::resizeEvent(QResizeEvent * e) {
	paintArea.setWidth( e->size().width()-paintAreaCorner.x()-10 );
	paintArea.setHeight( e->size().height()-paintAreaCorner.y()-10 );
}

void MainWindow::on_zoomIn_clicked() {
	++zoom;
	repaint();
}

void MainWindow::on_zoomOut_clicked() {
	--zoom;
	repaint();
}

void MainWindow::on_openImage_clicked() {
	imageName = QFileDialog::getOpenFileName( this, "Wybierz obraz", "." );
	if( srcImage.load( imageName ) ) {
		ui->startStop->setEnabled( true );
	} else {
		imageName="";
		QMessageBox::critical( this, "Błędny obraz", "Nie potrafię odczytać obrazu");
	}
	on_zoomNo_clicked();
}


void MainWindow::on_zoomNo_clicked() {
	zoom=reverseZoom( std::max( srcImage.width()/paintArea.width(), srcImage.height()/paintArea.height() )*0.9 );
	imageCenter.setX( srcImage.width()/2 );
	imageCenter.setY( srcImage.height()/2 );
	repaint();
}

void MainWindow::on_symSpeedVal_valueChanged(int value) {
	ui->symSpeedText->setText( QString("operacji/sekundę: %1").arg( pow(10,value/10.0), 0, 'f', 1 ) );
}

void MainWindow::on_symColorVal_valueChanged(int value) {
	ui->symColorText->setText( QString("zakres kolorowania: %1").arg( pow(10,value/10.0), 0, 'f', 0 ) );
	repaint();
}
