#ifndef ALG_HPP
#define ALG_HPP
#include <QPoint>
#include <QImage>
#include <QColor>

#include <QDebug>
#define DE qDebug()

class Alg
{
public:
	Alg();
	void clear();
	bool loadImage(const QString & imageName);
	void setStart( QPoint newStart );
	void setStop( QPoint newStop );

	QPoint start();
	QPoint stop();
	const QImage & printImage();

	bool step();
	void paint();

protected:
	QPoint start_;
	QPoint stop_;

	QImage srcImage_;
	QImage printImage_;
	static const int TRSHLD=5;
	static const int CLRMX=255;

};

#endif // ALG_HPP
