#ifndef ALG_HPP
#define ALG_HPP
#include <QPoint>
#include <QImage>
#include <QColor>

#include <QDebug>
#define DE qDebug()

class MyPoint {
public:
	QPoint p;
	int v;
};

class Alg
{
public:
	Alg();
	enum WorkMode { BreadthFirst, DepthFirst };
	void clear();
	bool loadImage(const QString & imageName);
	void setStart( QPoint newStart );
	void setStop( QPoint newStop );

	QPoint start();
	QPoint stop();
	const QImage & printImage();

	bool step();
	void paint();
	void paintAns();

	void setColorLength( int x );
	void setWorkMode( WorkMode workMode );

	static QRgb ScaleColorPercent( double val );

protected:
	QPoint start_;
	QPoint stop_;

	int colorLength_;

	WorkMode workMode_;

	QImage srcImage_;
	QImage printImage_;
	static const int TRSHLD=5;
	static const int CLRMX=255;

	QVector< MyPoint> parents_;
	MyPoint & parent_( int x, int y);
	MyPoint & parent_( const QPoint p );
	QVector< QPoint > queue_;
	QPoint queueNext_();
	QPoint workPoint_;
	void queueAdd_( QPoint x );

};

#endif // ALG_HPP
