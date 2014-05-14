#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
	paintAreaCorner( 210, 10), paintArea( paintAreaCorner.x(), paintAreaCorner.y(), 180, 180 ), zoom(1), imageCenter( 0, 0 ) {

	ui->setupUi(this);

	timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( nextStep() ) );
}

MainWindow::~MainWindow() {
	delete ui;
	delete timer;
}

void MainWindow::paintEvent(QPaintEvent * e) {
	if( e==Q_NULLPTR ) {
		;//NOOP
	}
	QPainter painter(this);

	painter.drawRect( paintArea );
	if( alg.printImage().isNull() ) {
		return;
	}
	const QImage & printImage=alg.printImage();
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

void MainWindow::startStopPoints(QPoint start, QPoint stop) {
	QString text;
	if( start.x() == -1 ) {
		text="Start: (x,x), ";
	} else {
		text=QString("Start: (%1,%2), ").arg( start.x() ).arg( start.y() );
	}
	if( stop.x() == -1 ) {
		text+="stop: (x,x), ";
	} else {
		text+=QString("stop: (%1,%2)").arg( stop.x() ).arg( stop.y() );
	}
	ui->startStopPoints->setText( text );
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

			int x=paintArea.width()/realZoom( zoom );
			int y=paintArea.height()/realZoom( zoom );

			QPoint mv( imageCenter.x()-x/2, imageCenter.y()-y/2 );

			paint.moveTo( ( paint.topLeft()-paintAreaCorner )/realZoom( zoom )+mv );

			paint.setWidth( paint.width()/realZoom( zoom ) );
			paint.setHeight( paint.height()/realZoom( zoom ) );

			if( ui->paintStart->isChecked() ) {
				alg.setStart( QPoint( paint.x()+paint.width(), paint.y()+paint.height() ) );
				startStopPoints( alg.start(), alg.stop() );
			} else if( ui->paintStop->isChecked() ) {
				alg.setStop( QPoint( paint.x()+paint.width(), paint.y()+paint.height() ) );
				startStopPoints( alg.start(), alg.stop() );
			}


	//		QPainter p( & alg.srcImage_);

	//		p.setBrush( QColor( 255, 0, 0 ) );
	//		p.drawRect( paint );
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
	if( alg.loadImage( imageName ) ) {
		ui->startStop->setEnabled( true );
		startStopPoints( alg.start(), alg.stop() );
	} else {
		imageName="";
		QMessageBox::critical( this, "Błędny obraz", "Nie potrafię odczytać obrazu");
	}
	on_zoomNo_clicked();
}


void MainWindow::on_zoomNo_clicked() {
	zoom=reverseZoom( std::min( paintArea.width()/((double)alg.printImage().width()), paintArea.height()/((double)alg.printImage().height()) )*0.99 );
	imageCenter.setX( alg.printImage().width()/2 );
	imageCenter.setY( alg.printImage().height()/2 );
	repaint();
	DE << zoom;
}

void MainWindow::on_symSpeedVal_valueChanged(int value) {
	stepTimeMsec=1000/pow(10,value/10.0);
	if( stepTimeMsec < 33 ) {
		stepMul=33/stepTimeMsec;
		stepTimeMsec=33;
	} else {
		stepMul=1;
	}
	if( timer->isActive() ) {
		timer->stop();
		timer->start( stepTimeMsec );
	}
	ui->symSpeedText->setText( QString("operacji/sekundę: %1").arg( pow(10,value/10.0), 0, 'f', 1 ) );
}

void MainWindow::on_symColorVal_valueChanged(int value) {
	alg.setColorLength( pow(10, value/10.0) );
	ui->symColorText->setText( QString("zakres kolorowania: %1").arg( pow(10,value/10.0), 0, 'f', 0 ) );
	repaint();
}

void MainWindow::nextStep() {
	for( int i=0; i<stepMul; ++i ) {
		if( alg.step() == false ) {
			on_startStop_clicked();//koniec pracy, auto-stop
			DE << stepTimeMsec << stepMul;
			break;
		}
	}
	alg.paint();
	repaint();
}

void MainWindow::on_startStop_clicked() {
	if( ui->startStop->text() == "Start" ) {
		ui->startStop->setText( "Stop" );
		ui->openImage->setDisabled( true );
		ui->alg->setDisabled( true );
		ui->paint->setDisabled( true );

		if( ui->depthFirst->isChecked() ) {
			alg.setWorkMode( alg.DepthFirst );
		} else {
			alg.setWorkMode( alg.BreadthFirst );
		}

		alg.setColorLength( pow( 10, ui->symColorVal->value()/10.0 ) );
		stepTimeMsec=1000/pow( 10, ui->symSpeedVal->value()/10.0 );

		if( stepTimeMsec < 33 ) {
			stepMul=33/stepTimeMsec;
			stepTimeMsec=33;
		} else {
			stepMul=1;
		}

		timer->start( stepTimeMsec );

	} else {
		ui->startStop->setText( "Start" );
		ui->openImage->setDisabled( false );
		ui->alg->setDisabled( false );
		ui->paint->setDisabled( false );

		timer->stop();

	}
}
