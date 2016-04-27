#include "c_colorforms.h"

CAbstractColorForm::CAbstractColorForm(const QRectF & aRect, const QColor & aColor, qreal anAngle)
    : rect(aRect),
      color(aColor),
      angle(anAngle)
{

}

/* ----------------------------------------------------------------------- */

CRectColorForm::CRectColorForm(const QRectF & aRect, const QColor & aColor, qreal anAngle)
    : CAbstractColorForm(aRect, aColor, anAngle)
{

}

void CRectColorForm::paintTo(QPainter *painter)
{
    if (painter) {
        QRect drawArea = painter->window();
        int x = qRound(rect.left() * drawArea.width());
        int y = qRound(rect.top() * drawArea.height());
        int w = qRound(rect.width() * drawArea.width());
        int h = qRound(rect.height() * drawArea.height());
        painter->setPen(color);
        painter->setBrush(color);
        if (angle == 0) {
            painter->drawRect(x, y, w, h);
        } else {
            painter->save();
            painter->translate(x + w/2, x + h/2);
            painter->rotate(angle);
            painter->drawRect(-(x + w/2), -(y + h/2), w, h);
            painter->restore();
        }
    }
}

CAbstractColorForm * CRectColorForm::clone()
{
    return new CRectColorForm(rect, color, angle);
}

/* ----------------------------------------------------------------------- */

CEllipseColorForm::CEllipseColorForm(const QRectF & aRect, const QColor & aColor, qreal anAngle)
    : CAbstractColorForm(aRect, aColor, anAngle)
{

}

void CEllipseColorForm::paintTo(QPainter *painter)
{
    if (painter) {
        QRect drawArea = painter->window();
        int x = qRound(rect.left() * drawArea.width());
        int y = qRound(rect.top() * drawArea.height());
        int w = qRound(rect.width() * drawArea.width());
        int h = qRound(rect.height() * drawArea.height());
        painter->setPen(color);
        painter->setBrush(color);
        if (angle == 0) {
            painter->drawEllipse(x, y, w, h);
        } else {
            painter->save();
            painter->translate(x + w/2, x + h/2);
            painter->rotate(angle);
            painter->drawEllipse(-(x + w/2), -(y + h/2), w, h);
            painter->restore();
        }
    }
}

CAbstractColorForm * CEllipseColorForm::clone()
{
    return new CEllipseColorForm(rect, color, angle);
}
