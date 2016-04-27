#ifndef CABSTRACTCOLORFORM_H
#define CABSTRACTCOLORFORM_H


#include <QRectF>
#include <QColor>
#include <QPainter>

class CAbstractColorForm
{
public:
    CAbstractColorForm(const QRectF & aRect, const QColor & aColor, qreal anAngle = 0);
    virtual ~CAbstractColorForm(){}
    virtual void paintTo(QPainter *painter) = 0;
    virtual CAbstractColorForm * clone() = 0;


    QRectF rect;
    qreal  angle;
    QColor color;
};


class CRectColorForm: public CAbstractColorForm
{
public:
    CRectColorForm(const QRectF & aRect, const QColor & aColor, qreal anAngle = 0);
    void paintTo(QPainter *painter);
    CAbstractColorForm * clone();
};


class CEllipseColorForm: public CAbstractColorForm
{
public:
    CEllipseColorForm(const QRectF & aRect, const QColor & aColor, qreal anAngle = 0);
    void paintTo(QPainter *painter);
    CAbstractColorForm * clone();
};



#endif // CABSTRACTCOLORFORM_H
