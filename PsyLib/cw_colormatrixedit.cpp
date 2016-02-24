#include "cw_colormatrixedit.h"
#include <QtDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

CColorMatrixEdit::CColorMatrixEdit(QWidget *parent)
    : CAbstractEdit(parent)
{
    matrix.setSize(1, 1);
    activeCell = QPoint(-1, -1);
    setMouseTracking(true);
}

void CColorMatrixEdit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if (matrix.isEmpty()) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(this->rect());
        return;
    }

    int w = width() / matrix.colCount();
    int h = height() / matrix.rowCount();

    painter.setPen(QPen(Qt::black, 3));
    for (int col = 0; col < matrix.colCount(); ++ col) {
        for (int row = 0; row < matrix.rowCount(); ++row) {
            painter.setBrush(matrix.getColor(col, row));
            painter.drawRect(col * w, row * h, w, h);
        }
    }

    painter.setBrush(Qt::NoBrush);
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    if (activeCell.x() >= 0 && activeCell.y() >= 0) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(activeCell.x() * w + 3, activeCell.y() * h + 3, w - 4, h - 4);
    }

    painter.end();
}


void CColorMatrixEdit::fillRandom(void)
{
    for (int col = 0; col < matrix.colCount(); ++ col)
        for (int row = 0; row < matrix.rowCount(); ++row) {
            matrix.setColor(col, row, QColor(qrand()%150 + 100, qrand()%150 + 100, qrand()%150 + 100));
        }
    this->update();
}

void CColorMatrixEdit::clear()
{
    for (int col = 0; col < matrix.colCount(); ++ col)
        for (int row = 0; row < matrix.rowCount(); ++row) {
            matrix.setColor(col, row, Qt::white);
        }
    this->update();
}

QPoint CColorMatrixEdit::cellAt(QPoint point)
{
    QPoint result = QPoint(-1, -1);
    if (point.x() >= 0 && point.x() < this->width() &&
        point.y() >= 0 && point.y() < this->height())
    {
        int w = width() / matrix.colCount();
        int h = height() / matrix.rowCount();
        return QPoint(point.x() / w, point.y() / h);
    }

    return QPoint(-1, -1);
}

void CColorMatrixEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint cell = cellAt(event->pos());
    if (cell.x() >=0 && cell.y() >= 0) {
        QColor color = matrix.getColor(cell.x(), cell.y());
        color = QColorDialog::getColor(color, this);
        if (color.isValid()) {
            matrix.setColor(cell.x(), cell.y(), color);
            this->update();
        }
    }
}

void CColorMatrixEdit::mouseMoveEvent(QMouseEvent *event)
{
    QPoint lastCell = activeCell;
    activeCell = cellAt(event->pos());
    if (activeCell != lastCell)
        this->update();
}

void CColorMatrixEdit::leaveEvent(QEvent *event)
{
    activeCell = QPoint(-1, -1);
    this->update();
}
