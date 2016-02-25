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
    currentCell = QPoint(0, 0);
    setMouseTracking(true);
    margin = 2;
    m_pixra = new CColorMatrixPixra;
    colorWheel = new ColorWheel(this);
    connect(colorWheel, SIGNAL(colorChanged(QColor)), SLOT(wheelColorChanged(QColor)));
}

CColorMatrixEdit::~CColorMatrixEdit()
{
    delete m_pixra;
}

void CColorMatrixEdit::setColorMatrix(const CColorMatrix & cm)
{
    matrix = cm;
    currentCell = QPoint(0, 0);
    activeCell = QPoint(-1, -1);
    updateMatrixArea();
    updateColorWheel();
}

void CColorMatrixEdit::updateMatrixArea()
{
    int w = width();
    int h = height();
    QRect drawArea = rect().adjusted(0, 0, -w/3 + 15, -h/3 + 15); //QRect(w/3, 0, w*2/3, h*2/3);
    matrixArea = drawArea.adjusted(0, 0, -4, -4);
    if (matrixArea.width() > matrixArea.height()) {
        matrixArea.setWidth(matrixArea.height());
    } else {
        matrixArea.setHeight(matrixArea.width());
    }
    int dx = drawArea.width() - matrixArea.width();
    int dy = drawArea.height() - matrixArea.height();

    int ddx = 0;
    int ddy = 0;
    if (!matrix.isEmpty()) {
        if (matrixArea.width() % matrix.colCount())
            ddx = 1;
        if (matrixArea.height() % matrix.rowCount())
            ddy = 1;
    }
    matrixArea.translate(dx/2 + ddx, dy/2 + ddy);

    colorWheel->setGeometry(w*2/3 - 5, h*2/3 - 5, w/3, h/3);

    update();
}

void CColorMatrixEdit::updateColorWheel()
{
    if(isCellValid(currentCell)) {
        colorWheel->setEnabled(true);
        colorWheel->setColor(matrix.getColor(currentCell.x(), currentCell.y()));
    } else {
        colorWheel->setEnabled(false);
    }
}

void CColorMatrixEdit::resizeEvent(QResizeEvent *)
{
    updateMatrixArea();
}

void CColorMatrixEdit::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#272822"));
    painter.drawRect(this->rect());

    int w = matrixArea.width() / matrix.colCount();
    int h = matrixArea.height() / matrix.rowCount();
    int dx = matrixArea.left();
    int dy = matrixArea.top();

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

//    if (activeCell.x() >= 0 && activeCell.y() >= 0) {
//        painter.setPen(QPen(Qt::red, 2));
//        painter.drawRect(activeCell.x() * w + dx, activeCell.y() * h + dy, w, h);
//    }

    if (isCellValid(currentCell)) {
        QColor currColor = matrix.getColor(currentCell.x(), currentCell.y());
        QColor cursorColor = Qt::white;
        if (currColor.value() > 127) {
            cursorColor = Qt::black;
        }
        cursorColor.setAlpha(100);

        int csz = 20;
        int cszX = currentCell.x() * w + dx + (w - csz)/2;
        int cszY = currentCell.y() * h + dy + (h - csz)/2;
        QRect cursorRect = QRect(cszX, cszY, csz, csz);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(cursorColor, 3));
        painter.drawEllipse(cursorRect);
    }

    painter.end();
}


void CColorMatrixEdit::fillRandom(void)
{
    for (int col = 0; col < matrix.colCount(); ++ col)
        for (int row = 0; row < matrix.rowCount(); ++row) {
            matrix.setColor(col, row, QColor(qrand()%150 + 100, qrand()%150 + 100, qrand()%150 + 100));
        }
    updateColorWheel();
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
        updateMatrixArea();
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
    if (matrixArea.contains(point))
    {
        int w = matrixArea.width() / matrix.colCount();
        int h = matrixArea.height() / matrix.rowCount();
        QPoint result = QPoint((point.x() - matrixArea.left()) / w, (point.y() - matrixArea.top()) / h);
        if (result.x() >= matrix.colCount())
            result.setX(-1);
        if (result.y() >= matrix.rowCount())
            result.setY(-1);
        return result;
    }

    return QPoint(-1, -1);
}

bool CColorMatrixEdit::isCellValid(const QPoint &cell)
{
    if (!matrix.isEmpty() && cell.x() >=0 && cell.y() >= 0 &&
         cell.x() < matrix.colCount() && cell.y() < matrix.rowCount())
    {
        return true;
    } else
        return false;
}

void CColorMatrixEdit::wheelColorChanged(const QColor &color)
{
    if (isCellValid(currentCell)) {
        matrix.setColor(currentCell.x(), currentCell.y(), color);
        update();
    }
}

void CColorMatrixEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint cell = cellAt(event->pos());
    if (isCellValid(cell)) {
        QColor color = matrix.getColor(cell.x(), cell.y());
        color = QColorDialog::getColor(color, this);
        if (color.isValid()) {
            matrix.setColor(cell.x(), cell.y(), color);
            this->update();
        }
    }
}

void CColorMatrixEdit::mousePressEvent(QMouseEvent *event)
{
    QPoint cell = cellAt(event->pos());
    if (isCellValid(cell)) {
        currentCell = cell;
        updateColorWheel();
        update();
    }
}

void CColorMatrixEdit::mouseMoveEvent(QMouseEvent *event)
{
    QPoint lastCell = activeCell;
    activeCell = cellAt(event->pos());
    if (activeCell != lastCell)
        this->update();
}

void CColorMatrixEdit::leaveEvent(QEvent *)
{
    activeCell = QPoint(-1, -1);
    this->update();
}
