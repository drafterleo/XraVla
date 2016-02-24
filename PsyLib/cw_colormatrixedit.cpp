#include "cw_colormatrixedit.h"
#include "math.h"
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
    margin = 2;
    m_pixra = new CColorMatrixPixra;
}

CColorMatrixEdit::~CColorMatrixEdit()
{
    delete m_pixra;
}

void CColorMatrixEdit::setColorMatrix(const CColorMatrix & cm)
{
    matrix = cm;
    updateDrawArea();
}

void CColorMatrixEdit::updateDrawArea()
{
    drawArea = this->rect().adjusted(0, 0, -6, -6);
    if (drawArea.width() > drawArea.height()) {
        drawArea.setWidth(drawArea.height());
    } else {
        drawArea.setHeight(drawArea.width());
    }
    int dx = this->rect().width() - drawArea.width();
    int dy = this->rect().height() - drawArea.height();

    int ddx = 0;
    int ddy = 0;
    if (!matrix.isEmpty()) {
        if (drawArea.width() % matrix.colCount())
            ddx = 1;
        if (drawArea.height() % matrix.rowCount())
            ddy = 1;
    }
    drawArea.translate(dx/2 + ddx, dy/2 + ddy);

    update();
}

void CColorMatrixEdit::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    updateDrawArea();
}

void CColorMatrixEdit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#272822"));
    painter.drawRect(this->rect());

    int w = drawArea.width() / matrix.colCount();
    int h = drawArea.height() / matrix.rowCount();
    int dx = drawArea.left();
    int dy = drawArea.top();

    painter.setPen(QPen(Qt::black, 2));
    for (int col = 0; col < matrix.colCount(); ++ col) {
        for (int row = 0; row < matrix.rowCount(); ++row) {
            painter.setBrush(matrix.getColor(col, row));
            QRect cellRect = QRect(col * w + dx, row * h + dy, w, h).adjusted(margin, margin, -margin, -margin);
            painter.drawRect(cellRect);
        }
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    if (activeCell.x() >= 0 && activeCell.y() >= 0) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(activeCell.x() * w + dx, activeCell.y() * h + dy, w, h);
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

bool CColorMatrixEdit::assignPixra(CAbstractPixra *pixra)
{
    CColorMatrixPixra *cmPixra = dynamic_cast <CColorMatrixPixra *>(pixra);
    if (cmPixra) {
        matrix = cmPixra->colorMatrix();
        updateDrawArea();
        return true;
    }
    return false;
}

CAbstractPixra* CColorMatrixEdit::pixra(void)
{
    m_pixra->setMatrix(this->matrix);
    return m_pixra;
}

QPoint CColorMatrixEdit::cellAt(QPoint point)
{
    if (drawArea.contains(point))
    {
        int w = drawArea.width() / matrix.colCount();
        int h = drawArea.height() / matrix.rowCount();
        QPoint result = QPoint((point.x() - drawArea.left()) / w, (point.y() - drawArea.top()) / h);
        if (result.x() >= matrix.colCount())
            result.setX(-1);
        if (result.y() >= matrix.rowCount())
            result.setY(-1);
        return result;
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
    Q_UNUSED(event)
    activeCell = QPoint(-1, -1);
    this->update();
}
