#include "cw_protopixrapopup.h"
#include "c_pixrafactory.h"
#include <QPainter>
#include <QMouseEvent>

CProtoPixraPopup::CProtoPixraPopup(QWidget *parent) :
    QWidget(parent)
{
    m_pixraWidth = 50;
    m_pixraHeight = 50;
    m_span = 10;
    m_hotPixraIdx = -1;
    m_pressPixraIdx = -1;

    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setMouseTracking(true);

    CFiguresPixra proto3Rects;
    proto3Rects.addRectangle(QRectF(0.1, 0.1, 0.33, 0.33));
    proto3Rects.addRectangle(QRectF(0.33, 0.33, 0.33, 0.33));
    proto3Rects.addRectangle(QRectF(0.56, 0.56, 0.33, 0.33));

    CFiguresPixra proto2RectsEllipse;
    proto2RectsEllipse.addRectangle(QRectF(0.1, 0.1, 0.33, 0.33));
    proto2RectsEllipse.addEllipse(QRectF(0.33, 0.33, 0.33, 0.33));
    proto2RectsEllipse.addRectangle(QRectF(0.56, 0.56, 0.33, 0.33));

    CFiguresPixra polygones5_3;
    polygones5_3.addPolygon(5, QRectF(0.1, 0.1, 0.8, 0.8));
    polygones5_3.addPolygon(3, QRectF(0.25, 0.25, 0.5, 0.5));

    CFiguresPixra polygone6Ellipse;
    polygone6Ellipse.addPolygon(6, QRectF(0.1, 0.1, 0.8, 0.8));
    polygone6Ellipse.addEllipse(QRectF(0.33, 0.33, 0.33, 0.33));


    CColorMatrixPixra proto3x3ColorMatrix;
    proto3x3ColorMatrix.setMatrix(CColorMatrix(3, 3));

    addProtoPixra(&proto3Rects);
    addProtoPixra(&proto2RectsEllipse);
    addProtoPixra(&polygones5_3);
    addProtoPixra(&polygone6Ellipse);
    addProtoPixra(&proto3x3ColorMatrix);
}

CProtoPixraPopup::~CProtoPixraPopup()
{
    for (int i = 0; i < m_protoPixras.count(); ++i) {
        delete m_protoPixras[i];
    }
}

void CProtoPixraPopup::addProtoPixra(CAbstractPixra *pixra)
{
    if (pixra) {
        CAbstractPixra *protoPixra = pixraFactoryInstance.createPixra(pixra->metaObject()->className());
        protoPixra->assign(pixra);
        m_protoPixras.append(protoPixra);
        m_rects.append(QRect());
        updateRects();
    }
}

void CProtoPixraPopup::updateRects()
{
    resize(m_span + (m_pixraWidth + m_span) * m_rects.count(), m_span * 2 + m_pixraHeight);
    for (int i = 0; i < m_rects.count(); ++i) {
        m_rects[i].setRect(m_span + (m_pixraWidth + m_span) * i, m_span, m_pixraWidth, m_pixraHeight);
        m_protoPixras[i]->resize(m_rects[i].size());
    }
    update();
}

void CProtoPixraPopup::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.setPen(QPen(QBrush(QColor(0x538BC4)), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));

    for (int i = 0; i < m_protoPixras.count(); ++i) {
        //painter.drawRect(m_rects[i]);
        CAbstractPixra *pixra = m_protoPixras[i];
        if (i == m_hotPixraIdx) {
            pixra->setColor(Qt::white);
            pixra->setFrameColor(QColor(0x538BC4));
        } else {
            pixra->setColor(Qt::lightGray);
            pixra->setFrameColor(QColor(0x466A7E));
        }
        //pixra->setBackColor();

        painter.save();
        painter.translate(m_rects[i].topLeft());
        pixra->render(&painter, QPoint(), QRegion(), 0);
        painter.restore();
    }

    painter.end();
}

void CProtoPixraPopup::mouseMoveEvent(QMouseEvent *event)
{
    int oldHotPixraIdx = m_hotPixraIdx;
    m_hotPixraIdx = -1;
    for (int i = 0; i < m_rects.count(); ++i) {
        if (m_rects[i].contains(event->pos())) {
            m_hotPixraIdx = i;
            break;
        }
    }

    if (m_hotPixraIdx != oldHotPixraIdx)
        update();
}

void CProtoPixraPopup::mousePressEvent(QMouseEvent *event)
{

    for (int i = 0; i < m_rects.count(); ++i) {
        if (m_rects[i].contains(event->pos())) {
            hide();
            emit pixraSelected(m_protoPixras[i]);
            return;
        }
    }

    QWidget::mousePressEvent(event);
}
