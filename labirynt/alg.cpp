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
					stop_.setX( x );
					stop_.setY( y );
				} else if( qRed( pix )<TRSHLD && qGreen( pix )>CLRMX-TRSHLD && qBlue( pix )<TRSHLD ) {
					start_.setX( x );
					start_.setY( y );
				}
				srcImage_.setPixel( x, y, white );
				++whitePointsNum_;
			}
		}
	}
	MyPoint errP;
	errP.p.setX( -1 );
	errP.p.setY( -1 );

	parents_.fill( errP, srcImage_.width()*srcImage_.height() );
	if( start_.x() != -1 ) {
		srcImage_.setPixel( start_, green );
		queue_.append( start_ );
		parent_( start_ ).v=0;
		parent_( start_ ).p.setX(-2);
		workPoint_=start_;
	}
	if( stop_.x() != -1 ) {
		srcImage_.setPixel( stop_, red );
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
		workPoint_=start_;
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
		if( i==0 ) {//left
			newPoint.setX( newPoint.x()-1 );
		} else if( i==1 ) {//up
			newPoint.setY( newPoint.y()-1 );
		} else if( i==2 ) {//right
			newPoint.setX( newPoint.x()+1 );
		} else {//down
			newPoint.setY( newPoint.y()+1 );
		}

		if( srcImage_.rect().contains( newPoint) && srcImage_.pixel( newPoint )!=black ) {
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
	srcImage_.setPixel( workPoint_, yellow );
	return true;
}

void Alg::paint() {
	printImage_=srcImage_;
	QRgb path=QColor( 0,255,255 ).rgb();
	QPoint printPoint=workPoint_;
	while( parent_( printPoint ).p.x()!=-1 && parent_( printPoint ).p.x()!=-2 ) {
		printImage_.setPixel( printPoint, path );
		printPoint=parent_( printPoint ).p;
	}
	int len=std::min( colorLength_, queue_.size() );
	for( int i=1; i<len; ++i ) {
		printImage_.setPixel( queue_[i], ScaleColorPercent( 100-100.0*i/len ) );
	}
}

void Alg::paintAns() {
	printImage_=srcImage_;
	QRgb path=QColor( 255,0,255 ).rgb();
	QPoint printPoint=stop_;
	while( parent_( printPoint ).p.x()!=-1 && parent_( printPoint ).p.x()!=-2 ) {
		printPoint=parent_( printPoint ).p;
		if( parent_( printPoint ).p.x()!=-2 ) {
			printImage_.setPixel( printPoint, path );
		}
	}

	QRgb red=QColor( 255,0,0 ).rgb();
	QRgb green=QColor( 0,255,0 ).rgb();
	printImage_.setPixel( start_, green );
	printImage_.setPixel( stop_, red );
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

MyPoint & Alg::parent_(int x, int y) {
	return parents_[ x*srcImage_.height()+y ];
}

MyPoint &Alg::parent_(const QPoint p) {
	return parents_[ p.x()*srcImage_.height()+p.y() ];
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
