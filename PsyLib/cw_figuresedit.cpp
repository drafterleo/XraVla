#include "cw_figuresedit.h"
#include "cw_figurespixra.h"
#include <qmath.h>
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>
#include <QTime>
#include <QKeyEvent>

CFiguresEdit::CFiguresEdit(QWidget * parent)
    : CAbstractEdit(parent)
{
    selectedFigure = -1;
    hotFigure = -1;
    catchedFigure = -1;
    catchedAnchor = -1;
    anchorRadius = 5;
    isMousePressed = false;

    m_pixra = new CFiguresPixra;
    m_clipboardPixra = new CFiguresPixra;

    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}


CFiguresEdit::~CFiguresEdit()
{
    clearFigures();
    clearHIstory();
    delete m_pixra;
    delete m_clipboardPixra;
}


void CFiguresEdit::resizeEvent(QResizeEvent *event)
{
    // qDebug() << event->oldSize() << event->size();

    qreal sx = static_cast<qreal>(width());
    qreal sy = static_cast<qreal>(height());

    if (!event->oldSize().isEmpty()) {
        sx = sx / event->oldSize().width();
        sy = sy / event->oldSize().height();
        for (int i = 0; i < figures.count(); ++i) {
            figures.at(i)->transform(QTransform().scale(sx, sy));
        }
    }
}


void CFiguresEdit::contentChanged()
{
    emit modified();
}


void CFiguresEdit::clearFigures()
{
    for (int i = 0; i < figures.count(); ++i) {
        delete figures[i];
    }
    figures.clear();

    selectedFigure = -1;
    selCluster.clear();
    hotFigure = -1;
    catchedFigure = -1;
    catchedAnchor = -1;

    update();
}


void CFiguresEdit::clear()
{
    clearFigures();
    contentChanged();
}

void CFiguresEdit::clearHIstory()
{
    while (!m_history.empty()) {
        delete m_history.pop();
    }
}


void CFiguresEdit::addHistory()
{
    if (figures.count() > 0){
        CFiguresPixra *pixra = new CFiguresPixra;
        this->writeToPixra(pixra);
        m_history.push(pixra);
    }
}


void CFiguresEdit::undo()
{
    if (!m_history.empty()) {
        CFiguresPixra *pixra = m_history.pop();
        if (pixra) {
            this->readFromPixra(pixra);
            delete pixra;
        }
    }
}


bool CFiguresEdit::writeToPixra(CAbstractPixra *pixra)
{
    CFiguresPixra *figPixra = dynamic_cast <CFiguresPixra *>(pixra);
    if (figPixra) {
        figPixra->clear();

        qreal sx = 1;
        qreal sy = 1;
        if (width() > 0)
            sx = 1 / static_cast<qreal>(width());
        if (height() > 0)
            sy = 1 / static_cast<qreal>(height());

        for (int i = 0; i < figures.count(); ++i) {
            CAbstractFigure *fig = figures.at(i)->clone();
            fig->transform(QTransform().scale(sx, sy));
            figPixra->addNormFigure(fig);
        }

        return true;
    }

    return false;
}


bool CFiguresEdit::readFromPixra(CAbstractPixra *pixra)
{
    CFiguresPixra *figPixra = dynamic_cast <CFiguresPixra *>(pixra);
    if (figPixra) {
        clearFigures();
        for (int i = 0; i < figPixra->normFigures().count(); ++i) {
            CAbstractFigure *fig = figPixra->normFigures().at(i)->clone();
            if (size().isValid())
                fig->transform(QTransform().scale(width(), height()));
            figures.append(fig);
        }
        contentChanged();
        update();
        return true;
    }
    return false;
}


bool CFiguresEdit::assignPixra(CAbstractPixra *pixra)
{
    addHistory();
    return readFromPixra(pixra);
}


CAbstractPixra *CFiguresEdit::pixra(void)
{
    this->writeToPixra(m_pixra);
    return m_pixra;
}


void CFiguresEdit::addFigure(CAbstractFigure *figure)
{
    if (figure) {
        addHistory();
        figure->setAnchorRadius(this->anchorRadius);
        figures.append(figure);
        contentChanged();
        this->update();
    }
}


void CFiguresEdit::deleteFigure(int idx)
{
    if (idx >= 0 && idx < figures.count())
    {
        addHistory();

        delete figures[idx];
        figures.remove(idx);

        if (idx == selectedFigure) {
            selectedFigure = -1;
            catchedFigure = -1;
        }

        contentChanged();
        this->update();
    }
}


void CFiguresEdit::addRandomPolygon(int n, int r)
{
    if (n > 2) {
        qsrand(QTime::currentTime().msec());
        CPolygonFigure *polygon = new CPolygonFigure;
        if (r > anchorRadius * n) {
            qreal pi = 3.14159265358979323;
            qreal angle = qrand() % 360 * (pi/180);
            qreal angleStep = (360/n) * (pi/180);
            for (int i = 0; i < n; ++i) {
                polygon->addPoint(QPointF(this->rect().center()) +
                                  QPointF(r * qCos(angle), r * qSin(angle)));
                angle += angleStep;
            }
        } else {
            for (int i = 0; i < n; ++i) {
                polygon->addPoint(QPointF(qrand()%width(), qrand()%height()));
            }
        }
        this->addFigure(polygon);
    }
}


void CFiguresEdit::addRectangle()
{
    int w = qrand()%(width() - 20) + 10;
    int h = qrand()%(height() - 20) + 10;
    int x = qrand()%(width() - w) + 4;
    int y = qrand()%(height() - h) + 4;
    addFigure(new CRectangleFigure(QRectF(x, y, w, h)));
}


void CFiguresEdit::addPolygon(int anchors)
{
    if (anchors >= 3) {
        addHistory();
        addRandomPolygon(anchors, qrand()%(width()/2 - 20) + 10);
    }
}


void CFiguresEdit::addEllipse()
{
    int w = qrand()%(width() - 20) + 10;
    int h = qrand()%(height() - 20) + 10;
    int x = qrand()%(width() - w) + 4;
    int y = qrand()%(height() - h) + 4;
    addFigure(new CEllipseFigure(QRectF(x, y, w, h)));
}


bool CFiguresEdit::isFigureSelected(int idx)
{
    return selCluster.contains(idx);
}


void CFiguresEdit::raiseFigure(int idx)
{
    //popup selected figure
    if (idx >= 0 && idx < figures.count() - 1) {
        CAbstractFigure *tmp = figures[idx];
        figures.remove(idx);
        figures.push_back(tmp);

        for (int i = 0; i < selCluster.count(); ++i) {
            if (selCluster.at(i) > idx)
                selCluster[i] --;
        }
//        if (selCluster.contains(idx)) {
//            selCluster.remove(selCluster.indexOf(idx));
//            selCluster.append(figures.count() - 1);
//        }

        //qDebug() << selCluster;

        int lastIdx = figures.count() - 1;
        if (selectedFigure == idx)
            selectedFigure = lastIdx;
        if (catchedFigure == idx)
            catchedFigure = lastIdx;
        if (hotFigure == idx)
            hotFigure = lastIdx;
    }

}


void CFiguresEdit::addSelectCluster(int idx)
{
    if (idx >= 0 && idx < figures.count() &&
        /*idx != selectedFigure && */
        !selCluster.contains(idx))
    {
        selCluster.append(idx);
        this->update();
    }
}


void CFiguresEdit::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (hasFocus())
        painter.setPen(QPen(QBrush(QColor(0x538BC4)), 2));
    else
        painter.setPen(QPen(QBrush(QColor(0x466A7E)), 2));
    painter.setBrush(QColor(0x272822));
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < figures.count(); ++i) {
        /* draw figure */
        painter.setBrush(Qt::NoBrush);
        if (i == selectedFigure) {
            painter.setPen(QPen(QBrush(QColor(0xF92659)), 3));
        } else
        if (selCluster.contains(i))
           painter.setPen(QPen(QBrush(QColor(0xF976A9)), 3));
        else
        if (i == hotFigure)
            painter.setPen(QPen(QBrush(QColor(0x66D9EF)), 3));
        else
            painter.setPen(QPen(QBrush(Qt::white), 3));
        painter.drawPath(figures.at(i)->getPath());

        /* draw anchors */
        if (i == selectedFigure) {
            QVector <QPointF> iAnchors = figures.at(i)->getAnchors();
            for (int j = 0; j < iAnchors.count(); ++j) {

                painter.setBrush(Qt::red);

                if (j == hotAnchor || j == catchedAnchor) {
                    painter.setPen(QPen(QBrush(QColor(0xFF7D7A)), 3));
                } else {
                    painter.setPen(QPen(QBrush(QColor(0xF92659)), 3));
                }

                painter.drawEllipse(iAnchors.at(j), anchorRadius, anchorRadius);
            }
        }
    }

    painter.end();
}


void CFiguresEdit::mousePressEvent(QMouseEvent *event)
{
    isMousePressed = true;
    pressPoint = event->pos();

    if (selectedFigure >= 0) {
        catchedAnchor = figures.at(selectedFigure)->anchorAt(pressPoint);
        if (catchedAnchor >= 0) {
            addHistory();
            this->update();
            return;
        }
        if (hotFigure < 0 && figures.at(selectedFigure)->getPath().contains(pressPoint)) {
            addHistory();
            catchedFigure = selectedFigure;
            return;
        }
    }

    int prevSelected = selectedFigure;
    int lastSpace = this->height() * this->width();
    catchedFigure = -1;
    selectedFigure = -1;
    for (int i = 0; i < figures.count(); ++i) {
        if (figures.at(i)->getPath().contains(pressPoint)) {
            QRect pathRect = figures.at(i)->getPath().controlPointRect().toRect();
            int newSpace = pathRect.height() * pathRect.width();
            if (newSpace < lastSpace) {
                lastSpace = newSpace;
                selectedFigure = i;
                catchedFigure = i;
            }
        }
    }

    if (catchedFigure >= 0) {
        addHistory();
    }

    if (event->modifiers() & Qt::ControlModifier &&
        prevSelected >= 0 &&
        prevSelected != selectedFigure)
    {
        if (selCluster.indexOf(selectedFigure) >= 0)
            selCluster.remove(selCluster.indexOf(selectedFigure));
        addSelectCluster(prevSelected);
    } else
        if (!(event->modifiers() & Qt::ControlModifier))
            selCluster.clear();

    //popup catched figure
    if (selectedFigure >=0 && selectedFigure != figures.count() - 1) {
        raiseFigure(selectedFigure);
        hotFigure = -1;
    }

    this->update();
}


void CFiguresEdit::mouseMoveEvent(QMouseEvent *event)
{
    int lastHotFigure = hotFigure;
    hotFigure = -1;
    hotAnchor = -1;

    if (selectedFigure >= 0 && catchedAnchor >= 0) {
        qreal ax = qBound(0, event->pos().x(), this->width());
        qreal ay = qBound(0, event->pos().y(), this->height());
        figures.at(selectedFigure)->moveAnchor(catchedAnchor, QPointF(ax, ay));
        contentChanged();
        this->update();
        return;
    }

    if (isMousePressed && catchedFigure >= 0) {
        int dx = event->pos().x() - pressPoint.x();
        int dy = event->pos().y() - pressPoint.y();
        //translateFigure(catchedFigure, dx, dy);
        translateSelection(dx, dy, false);
        pressPoint = event->pos();
        return;
    }

    if (selectedFigure >= 0) {
        hotAnchor = figures.at(selectedFigure)->anchorAt(event->pos());
        this->update();
        if (hotAnchor >=0)
            return;
    }

    int lastSpace = this->height() * this->width();
    for (int i = 0; i < figures.count(); ++i) {
        if (figures.at(i)->getPath().contains(event->pos() )) {
            QRect pathRect = figures.at(i)->getPath().controlPointRect().toRect();
            int newSpace = pathRect.height() * pathRect.width();
            if (newSpace < lastSpace) {
                lastSpace = newSpace;
                hotFigure = i;
            }
        }
    }

    if (hotFigure == selectedFigure)
        hotFigure = -1;
    if (hotFigure != lastHotFigure)
        this->update();
}


void CFiguresEdit::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    catchedAnchor = -1;
    catchedFigure = -1;
    isMousePressed = false;
    update();
}


bool CFiguresEdit::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = (QKeyEvent *)event;
      if (keyEvent->key() == Qt::Key_Tab && figures.count() > 0) {
          //qDebug() << keyEvent->modifiers();
          int selIdx;
          if (keyEvent->modifiers() & Qt::ShiftModifier) {
              selIdx = selectedFigure - 1;
              if (selIdx < 0)
                  selIdx = figures.count() - 1;
          } else {
              selIdx = selectedFigure + 1;
              if (selIdx >= figures.count())
                  selIdx = 0;
          }
          selectedFigure = selIdx;
          this->update();
          return true;
      }
    }
    return CAbstractEdit::event(event);
}


void CFiguresEdit::keyPressEvent(QKeyEvent *event)
{
    if (catchedFigure < 0) {
        if (event->modifiers() & Qt::ShiftModifier) {
            if (event->key() == Qt::Key_Up) {
                scaleFigure(selectedFigure, 1.05, 1.05);
            } else
            if (event->key() == Qt::Key_Down) {
                scaleFigure(selectedFigure, 0.95, 0.95);
            }
        } else
        if (event->modifiers() & Qt::ControlModifier) {
            if (event->key() == Qt::Key_C) {
                this->writeToPixra(m_clipboardPixra);
            } else
            if (event->key() == Qt::Key_V && !m_clipboardPixra->isEmpty()) {
                assignPixra(m_clipboardPixra);
            } else
            if (event->key() == Qt::Key_Z) {
                undo();
            }
        } else
        if (event->key() == Qt::Key_Left) {
            translateSelection(-1, 0);
        } else
        if (event->key() == Qt::Key_Right) {
            translateSelection(1, 0);
        } else
        if (event->key() == Qt::Key_Up) {
            translateSelection(0, -1);
        } else
        if (event->key() == Qt::Key_Down) {
            translateSelection(0, 1);
        } else
        if (event->key() == Qt::Key_Delete) {
            deleteFigure(selectedFigure);
        }
    }

    if (event->key() == Qt::Key_1) {
        addRectangle();
    } else
    if (event->key() == Qt::Key_3) {
        addPolygon(3);
    } else
    if (event->key() == Qt::Key_4) {
        addPolygon(4);
    } else
    if (event->key() == Qt::Key_5) {
        addPolygon(5);
    } else
    if (event->key() == Qt::Key_6) {
        addPolygon(6);
    } else
    if (event->key() == Qt::Key_7) {
        addPolygon(7);
    } else
    if (event->key() == Qt::Key_0) {
        addEllipse();
    }
}


void CFiguresEdit::translateFigure(int figIdx, int dx, int dy)
{
    if (figIdx >= 0 && figIdx < figures.count()) {
        QPainterPath path = figures.at(figIdx)->transformedPath(QTransform().translate(dx, dy));
        if (this->rect().contains(path.controlPointRect().toRect())) {
            figures.at(figIdx)->transform(QTransform().translate(dx, dy));
            contentChanged();
            this->update();
        }
    }
}


void CFiguresEdit::translateSelection(int dx, int dy, bool saveHistory)
{
    if (saveHistory) {
        addHistory();
    }

    if (selCluster.isEmpty()) {
        translateFigure(selectedFigure, dx, dy);
    } else {
        QVector <int> selTmp(selCluster);
        if (selectedFigure >= 0 && !selTmp.contains(selectedFigure))
            selTmp.append(selectedFigure);

        QRect pathRect;
        for (int i = 0; i < selTmp.count(); ++i) {
            pathRect = pathRect.united(figures.at(selTmp[i])->getPath().controlPointRect().toRect().translated(dx, dy));
        }
        if (this->rect().contains(pathRect)) {
            for (int i = 0; i < selTmp.count(); ++i) {
                translateFigure(selTmp[i], dx, dy);
            }
        }
    }
}


void CFiguresEdit::scaleFigure(int figIdx, qreal sx, qreal sy)
{
    if (figIdx >= 0 && figIdx < figures.count()) {
        QPainterPath path = figures.at(figIdx)->transformedPath(QTransform().scale(sx, sy));
        if (this->rect().contains(path.controlPointRect().toRect())) {
            addHistory();
            QPointF figCenter = figures.at(figIdx)->getPath().controlPointRect().center();
            QPointF transCenter = path.controlPointRect().center();
            figures.at(figIdx)->transform(QTransform().scale(sx, sy));
            translateFigure(figIdx, figCenter.x() - transCenter.x(), figCenter.y() - transCenter.y());
            contentChanged();
            this->update();
        }
    }
}


void CFiguresEdit::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    update();
}


void CFiguresEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    catchedAnchor = -1;
    catchedFigure = -1;
    selectedFigure = -1;
    update();
}


void CFiguresEdit::randomize()
{
    addHistory();
    for (int i = 0; i < figures.count(); ++i) {
        figures.at(i)->randomize(this->rect().adjusted(5, 5, -5, -5), 20);
    }
    emit modified();
    repaint();
}
