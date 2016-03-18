#include "cw_colormatrixedit.h"
#include "qmath.h"
#include <QtDebug>
#include <QPainter>
#include <QMouseEvent>


CColorMatrixEdit::CColorMatrixEdit(QWidget *parent)
    : CAbstractEdit(parent)
{
    matrix.setSize(1, 1);
    activeCell = QPoint(-1, -1);
    currentCell = QPoint(0, 0);
    pressedCell = QPoint(-1, -1);
    margin = 2;
    m_pixra = new CColorMatrixPixra;
    colorWheel = new ColorWheel(this);
    connect(colorWheel, SIGNAL(colorChanged(QColor)), SLOT(colorWheelChanged(QColor)));

    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    colorWheel->installEventFilter(this);
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
    commitModification();
}

void CColorMatrixEdit::commitModification()
{
    history.append(matrix);
    while (history.length() > 1000) {
        history.removeFirst();
    }
    emit modified();
}

void CColorMatrixEdit::undoModification()
{
    if (history.length() > 0) {
        if (history.length() > 1) { // save first matrix
            history.removeLast();
        }
        matrix = history.last();
        updateMatrixArea();
        updateColorWheel();
        emit modified();
    }
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
        colorWheel->setColor(matrix.getColor(currentCell));
    } else {
        colorWheel->setEnabled(false);
    }
}

void CColorMatrixEdit::resizeEvent(QResizeEvent *)
{
    updateMatrixArea();
}

void CColorMatrixEdit::rgbSums(int & r, int & g, int & b)
{
    r = 0;
    g = 0;
    b = 0;

    for (int col = 0; col < matrix.colCount(); ++ col) {
        for (int row = 0; row < matrix.rowCount(); ++row) {
            QColor color = matrix.getColor(col, row);
            r += color.red();
            g += color.green();
            b += color.blue();
        }
    }
}

void CColorMatrixEdit::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (hasFocus())
        painter.setPen(QPen(QBrush(QColor(0x538BC4)), 2));
    else
        painter.setPen(QPen(QBrush(QColor(0x466A7E)), 2));
    painter.setBrush(QColor(0x272822));
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

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

//    if (activeCell.x() >= 0 && activeCell.y() >= 0) {
//        painter.setPen(QPen(Qt::red, 2));
//        painter.drawRect(activeCell.x() * w + dx, activeCell.y() * h + dy, w, h);
//    }

    if (isCellValid(currentCell)) {
        QColor currColor = matrix.getColor(currentCell);
        QColor cursorColor = Qt::white;
        if (currColor.value() > 127) {
            cursorColor = Qt::black;
        }
        cursorColor.setAlpha(90);

        int csz = 20;
        int cszX = currentCell.x() * w + dx + (w - csz)/2;
        int cszY = currentCell.y() * h + dy + (h - csz)/2;
        QRect cursorRect = QRect(cszX, cszY, csz, csz);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(cursorColor, 3));
        painter.drawEllipse(cursorRect);
    }

//    int r, g, b;
//    rgbSums(r, g, b);
//    painter.setPen(Qt::lightGray);
//    painter.drawText(rect(), Qt::AlignBottom | Qt::AlignLeft, QString("r: %1, g: %2, b: %3").arg(r).arg(g).arg(b));

    painter.end();
}

QColor genRandomColor()
{
    //return QColor(qrand()%150 + 100, qrand()%150 + 100, qrand()%150 + 100);
    return QColor(qrand()%200 + qrand()%40 + 5,
                  qrand()%200 + qrand()%40 + 5,
                  qrand()%200 + qrand()%40 + 5);
}

void CColorMatrixEdit::fillRandom(void)
{
    qreal passDistance = 20;
    QVector <QColor> addedColors;
    for (int col = 0; col < matrix.colCount(); ++ col) {
        for (int row = 0; row < matrix.rowCount(); ++row) {
            QColor rndColor;
            bool passColor = false;
            int attempts = 100;
            while (passColor == false && attempts > 0) {
                passColor = true;
                rndColor = genRandomColor();
                for (int i = 0; i < addedColors.count(); ++i) {
                    QColor iColor = addedColors.at(i);
                    int dr = rndColor.red() - iColor.red();
                    int dg = rndColor.green() - iColor.green();
                    int db = rndColor.blue() - iColor.blue();
                    qreal distance = sqrt(dr*dr + dg*dg + db*db);
                    //qDebug() << distance;
                    if (distance < passDistance) {
                        passColor = false;
                        break;
                    }
                }
                attempts --;
            }
            addedColors.append(rndColor);
            matrix.setColor(col, row, rndColor);
        }
    }
    updateColorWheel();
    this->update();
    commitModification();
}

void CColorMatrixEdit::clear()
{
    for (int col = 0; col < matrix.colCount(); ++ col)
        for (int row = 0; row < matrix.rowCount(); ++row) {
            matrix.setColor(col, row, Qt::white);
        }
    this->update();
    updateColorWheel();
    commitModification();
}

bool CColorMatrixEdit::assignPixra(CAbstractPixra *pixra)
{
    CColorMatrixPixra *cmPixra = dynamic_cast <CColorMatrixPixra *>(pixra);
    if (cmPixra) {
        matrix = cmPixra->getMatrix();
        updateMatrixArea();
        updateColorWheel();
        commitModification();
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

void CColorMatrixEdit::colorWheelChanged(const QColor &color)
{
    if (isCellValid(currentCell) && matrix.getColor(currentCell) != color) {
        matrix.setColor(currentCell, color);
        update();
        commitModification();
    }
}

void CColorMatrixEdit::setCurrentCell(int col, int row)
{
    if (row >= 0 && col >= 0 && col < matrix.colCount() && row < matrix.rowCount()){
        currentCell = QPoint(col, row);
        updateColorWheel();
        update();
    }
}

void CColorMatrixEdit::setMatrixSize(int cols, int rows)
{
    if (matrix.size() != QSize(cols, rows) && cols > 0 && rows > 0) {
        matrix.setSize(cols, rows);
        currentCell = QPoint(0, 0);
        updateColorWheel();
        update();
        commitModification();
    }
}

bool CColorMatrixEdit::eventFilter(QObject *target, QEvent *event)
{
    if (target == colorWheel) {
        if (event->type() == QEvent::KeyPress) {
            this->keyPressEvent(static_cast<QKeyEvent *>(event));
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}


void CColorMatrixEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left) {
        setCurrentCell(currentCell.x() - 1, currentCell.y());
    } else
    if (event->key() == Qt::Key_Right) {
        setCurrentCell(currentCell.x() + 1, currentCell.y());
    } else
    if (event->key() == Qt::Key_Up) {
        setCurrentCell(currentCell.x(), currentCell.y() - 1);
    } else
    if (event->key() == Qt::Key_Down) {
        setCurrentCell(currentCell.x(), currentCell.y() + 1);
    } else
    if (event->key() == Qt::Key_1) {
        setMatrixSize(1, 1);
    } else
    if (event->key() == Qt::Key_2) {
        setMatrixSize(2, 2);
    } else
    if (event->key() == Qt::Key_3) {
        setMatrixSize(3, 3);
    } else
    if (event->key() == Qt::Key_4) {
        setMatrixSize(4, 4);
    } else
    if (event->key() == Qt::Key_5) {
        setMatrixSize(5, 5);
    } else
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Z) {
            undoModification();
        }
    }
}

void CColorMatrixEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint cell = cellAt(event->pos());
    if (isCellValid(cell)) {
        QColor color = matrix.getColor(cell);
        if (!colorDialogPos.isNull())
            colorDialog.move(colorDialogPos);
        colorDialog.setCurrentColor(color);
        if (colorDialog.exec() == QDialog::Accepted) {
            colorDialogPos = colorDialog.pos();
            color = colorDialog.currentColor();
            if (color.isValid()) {
                matrix.setColor(cell, color);
                update();
                updateColorWheel();
                commitModification();
            }
        }
    }
}

void CColorMatrixEdit::mousePressEvent(QMouseEvent *event)
{
    QPoint cell = cellAt(event->pos());
    if (isCellValid(cell)) {
        pressedCell = cell;
        currentCell = cell;
        updateColorWheel();
        update();
    }
}

void CColorMatrixEdit::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint cell = cellAt(event->pos());
    if (isCellValid(cell)) {
        currentCell = cell;
        if (isCellValid(pressedCell) && pressedCell != currentCell) {
            matrix.setColor(currentCell, matrix.getColor(pressedCell));
            commitModification();
        }
        pressedCell = QPoint(-1, -1);
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
