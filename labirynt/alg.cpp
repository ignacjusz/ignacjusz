#include "alg.hpp"

Alg::Alg() {
	clear();
}

void Alg::clear() {
	start_.setX(-1);
	start_.setY(-1);
	stop_.setX(-1);
	stop_.setY(-1);
	colorLength_=10;
	workMode_=DepthFirst;
	queue_.clear();
	whitePointsNum_=0;
	currentPointNum_=0;
}

bool Alg::loadImage( const QString & imageName )
{
	clear();
	if( ! srcColorImage_.load( imageName ) ) {
		return false;
	}
	srcColorImage_=srcColorImage_.convertToFormat( QImage::Format_RGB32 );
	QRgb black=QColor( 0,0,0 ).rgb();
	QRgb white=QColor( 255,255,255 ).rgb();
	QRgb red=QColor( 255,0,0 ).rgb();
	QRgb green=QColor( 0,255,0 ).rgb();
	for( int x=0; x<srcColorImage_.width(); ++x ) {
		for( int y=0; y<srcColorImage_.height(); ++y ) {

			QRgb pix=srcColorImage_.pixel( x, y );
			if( qRed( pix )<TRSHLD && qGreen( pix )<TRSHLD && qBlue( pix )<TRSHLD ) {//Black
				srcColorImage_.setPixel( x, y, black );
			} else {
				if( qRed( pix )>CLRMX-TRSHLD && qGreen( pix )<TRSHLD && qBlue( pix )<TRSHLD ) {
					stop_.setX( x );
					stop_.setY( y );
				} else if( qRed( pix )<TRSHLD && qGreen( pix )>CLRMX-TRSHLD && qBlue( pix )<TRSHLD ) {
					start_.setX( x );
					start_.setY( y );
				}
				srcColorImage_.setPixel( x, y, white );
				++whitePointsNum_;
			}
		}
	}
	MyPoint errP;
	errP.p.setX( -1 );
	errP.p.setY( -1 );

	parents_.fill( errP, srcColorImage_.width()*srcColorImage_.height() );
	if( start_.x() != -1 ) {
		srcColorImage_.setPixel( start_, green );
		queue_.append( start_ );
		parent_( start_ ).v=0;
		parent_( start_ ).p.setX(-2);
		workPoint_=start_;
	}
	if( stop_.x() != -1 ) {
		srcColorImage_.setPixel( stop_, red );
	}

	srcMonoImage_=srcColorImage_;
	paint(false);
	return true;
}

void Alg::setStart(QPoint newStart) {
	if( srcColorImage_.rect().contains( newStart ) && newStart!=stop_ ) {
		if( start_.x() != -1 ) {
			QRgb white=QColor( 255,255,255 ).rgb();
			srcColorImage_.setPixel( start_, white );
		}

		start_=newStart;
		QRgb green=QColor( 0,255,0 ).rgb();
		srcColorImage_.setPixel( start_, green );
		workPoint_=start_;
	}
	srcMonoImage_=srcColorImage_;
	paint(false);
}

void Alg::setStop(QPoint newStop) {
	if( srcColorImage_.rect().contains( newStop ) && newStop!=start_ ) {

		if( stop_.x() != -1 ) {
			QRgb white=QColor( 255,255,255 ).rgb();
			srcColorImage_.setPixel( stop_, white );
		}

		stop_=newStop;
		QRgb red=QColor( 255,0,0 ).rgb();
		srcColorImage_.setPixel( stop_, red );
	}
	srcMonoImage_=srcColorImage_;
	paint(false);
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

bool Alg::step() {
	if( queue_.isEmpty() ) {
		return false;
	}
	QRgb black=QColor( 0,0,0 ).rgb();
	++currentPointNum_;
	//QRgb yellow=QColor( 255,255, 255*currentPointNum_/whitePointsNum_ ).rgb();
	QRgb yellow=ScaleColorPercent( 100*currentPointNum_/whitePointsNum_ );

	workPoint_=queueNext_();
	QPoint newPoint;

	for( int i=0; i<4; ++i ) {

		newPoint=workPoint_;
		if( i==0 ) {//right
			newPoint.setX( newPoint.x()+1 );
		} else if( i==1 ) {//down
			newPoint.setY( newPoint.y()+1 );
		} else if( i==2 ) {//left
			newPoint.setX( newPoint.x()-1 );
		} else {//up
			newPoint.setY( newPoint.y()-1 );
		}

		if( srcColorImage_.rect().contains( newPoint) && srcColorImage_.pixel( newPoint )!=black ) {
			if( parent_( newPoint ).p.x()==-1 ) { //nie odwiedzany nigdy
				parent_( newPoint ).p=workPoint_;
				parent_( newPoint ).v=parent_( workPoint_ ).v+1;
				queueAdd_( newPoint );
			} else {
				if( parent_( newPoint ).v > parent_( workPoint_ ).v+1 ) {
					parent_( newPoint ).p=workPoint_;
					parent_( newPoint ).v=parent_( workPoint_ ).v+1;
				}
			}
		}
	}
	srcColorImage_.setPixel( workPoint_, yellow );
	return true;
}

int Alg::paint(bool colorBackground) {
	if( srcMonoImage_.isNull() ) {
		return -1;
	}
	int n=0;
	if( colorBackground ) {
		printImage_=srcColorImage_;
	} else {
		printImage_=srcMonoImage_;
	}
	QRgb path=QColor( 0,255,255 ).rgb();
	QPoint printPoint=workPoint_;
	while( parent_( printPoint ).p.x()!=-1 && parent_( printPoint ).p.x()!=-2 ) {
		printImage_.setPixel( printPoint, path );
		printPoint=parent_( printPoint ).p;
		++n;
	}
	int len=std::min( colorLength_, queue_.size() );
	for( int i=0; i<len; ++i ) {
		printImage_.setPixel( queue_[i], ScaleColorPercent( 100-100.0*i/len ) );
	}
	return n;
}

int Alg::paintAns(bool colorBackground) {
	if( srcMonoImage_.isNull() ) {
		return -1;
	}
	int n=0;
	if( colorBackground ) {
		printImage_=srcColorImage_;
	} else {
		printImage_=srcMonoImage_;
	}
	QRgb path=QColor( 255,0,255 ).rgb();
	QPoint printPoint=stop_;
	while( parent_( printPoint ).p.x()!=-1 && parent_( printPoint ).p.x()!=-2 ) {
		printPoint=parent_( printPoint ).p;
		printImage_.setPixel( printPoint, path );
		++n;
	}

	QRgb red=QColor( 255,0,0 ).rgb();
	QRgb green=QColor( 0,255,0 ).rgb();
	printImage_.setPixel( start_, green );
	printImage_.setPixel( stop_, red );
	return n;
}

void Alg::setColorLength(int x) {
	colorLength_=x;
}

void Alg::setWorkMode(Alg::WorkMode workMode) {
	workMode_=workMode;
}

QRgb Alg::ScaleColorPercent(double t) {
	//int t=(100-val)/2+50;
	int r,g,b;
	if( t < 0 ) {
	  r=0;
	  g=0;
	  b=64;
	} else if( t < 20 ) {
	  r=0;
	  g=0;
	  b=(t)*9.6+64;
	} else if( t < 40 ) {
	  r=0;
	  g=(t - 20)*255/20;
	  b=255;
	} else if( t < 50 ) {
	  r=0;
	  g=255;
	  b=(50 - t)*255/10;
	} else if( t < 60 ) {
	  r=(t - 50)*255/10;
	  g=255;
	  b=0;
	} else if( t < 80 ) {
	  r=255;
	  g=(80 - t)*255/20;
	  b=0;
	} else if( t < 100 ) {
	  r=(100 - t)*9.6+63;
	  g=0;
	  b=0;
	} else {
	  r=64;
	  g=0;
	  b=0;
	}
	return QColor( r, g, b ).rgb();
}

QPoint Alg::workPoint() {
	return workPoint_;
}

int Alg::workpointPathLength() {
	return parent_( workPoint_ ).v;
}

int Alg::currentPointNum() {
	return currentPointNum_;
}

int Alg::queueLength() {
	return queue_.size();
}

MyPoint & Alg::parent_(int x, int y) {
	return parents_[ x*srcColorImage_.height()+y ];
}

MyPoint &Alg::parent_(const QPoint p) {
	return parents_[ p.x()*srcColorImage_.height()+p.y() ];
}

QPoint Alg::queueNext_() {
	QPoint p=queue_.first();
	queue_.pop_front();
	return p;
}

void Alg::queueAdd_(QPoint x) {
	if( workMode_==DepthFirst ) {
		queue_.prepend( x );
	} else {
		queue_.append( x );
	}
}
