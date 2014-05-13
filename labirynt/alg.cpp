#include "alg.hpp"

Alg::Alg() {
	clear();
}

void Alg::clear() {
	start_.setX(-1);
	start_.setY(-1);
	stop_.setX(-1);
	stop_.setY(-1);
}

bool Alg::loadImage( const QString & imageName )
{
	clear();
	if( ! srcImage_.load( imageName ) ) {
		return false;
	}
	srcImage_=srcImage_.convertToFormat( QImage::Format_RGB32 );
	QRgb black=QColor( 0,0,0 ).rgb();
	QRgb white=QColor( 255,255,255 ).rgb();
	QRgb red=QColor( 255,0,0 ).rgb();
	QRgb green=QColor( 0,255,0 ).rgb();
	for( int x=0; x<srcImage_.width(); ++x ) {
		for( int y=0; y<srcImage_.height(); ++y ) {

			QRgb pix=srcImage_.pixel( x, y );
			if( qRed( pix )<TRSHLD && qGreen( pix )<TRSHLD && qBlue( pix )<TRSHLD ) {//Black
				srcImage_.setPixel( x, y, black );
			} else {
				if( qRed( pix )>CLRMX-TRSHLD && qGreen( pix )<TRSHLD && qBlue( pix )<TRSHLD ) {
					start_.setX( x );
					start_.setY( y );
				} else if( qRed( pix )<TRSHLD && qGreen( pix )>CLRMX-TRSHLD && qBlue( pix )<TRSHLD ) {
					stop_.setX( x );
					stop_.setY( y );
				}
				srcImage_.setPixel( x, y, white );
			}
		}
	}
	if( start_.x() != -1 ) {
		srcImage_.setPixel( start_, red );
	}
	if( stop_.x() != -1 ) {
		srcImage_.setPixel( stop_, green );
	}
	paint();
	return true;
}

void Alg::setStart(QPoint newStart) {
	if( srcImage_.rect().contains( newStart ) && newStart!=stop_ ) {
		if( start_.x() != -1 ) {
			QRgb white=QColor( 255,255,255 ).rgb();
			srcImage_.setPixel( start_, white );
		}

		start_=newStart;
		QRgb red=QColor( 255,0,0 ).rgb();
		srcImage_.setPixel( start_, red );
	}
	paint();
}

void Alg::setStop(QPoint newStop) {
	if( srcImage_.rect().contains( newStop ) && newStop!=start_ ) {

		if( stop_.x() != -1 ) {
			QRgb white=QColor( 255,255,255 ).rgb();
			srcImage_.setPixel( stop_, white );
		}

		stop_=newStop;
		QRgb green=QColor( 0,255,0 ).rgb();
		srcImage_.setPixel( stop_, green );
	}
	paint();
}

QPoint Alg::start() {
	return start_;
}

QPoint Alg::stop() {
	return stop_;
}

const QImage &Alg::printImage() {
	return printImage_;
}

bool Alg::step() {//TODO

}

void Alg::paint() {//TODO:
	printImage_=srcImage_;
}
