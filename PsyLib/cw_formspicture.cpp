#include "cw_formspicture.h"
#include <QPainter>

CColorFormsPicture::CColorFormsPicture(QWidget *parent) : QWidget(parent)
{

}

CColorFormsPicture::~CColorFormsPicture()
{
    clearColorForms();
}

void CColorFormsPicture::clearColorForms()
{
    while (m_colorForms.isEmpty() == false) {
        delete m_colorForms.takeFirst();
    }
    update();
}

void CColorFormsPicture::addForm(CAbstractColorForm * form)
{
    m_colorForms.append(form);
    update();
}

void CColorFormsPicture::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (int i = 0; i < m_colorForms.count(); ++i) {
        m_colorForms.at(i)->paintTo(&painter);
    }
    painter.end();
}

QColor rndColor()
{
    int r = qrand() % 255;
    int g = qrand() % 255;
    int b = qrand() % 255;
    int alpha = qrand() % 100 + 155;
    return QColor(r, g, b, alpha);
}

QRectF rndRect(int minW, int minH, int maxW, int maxH, qreal scalar)
{
    int w = qrand() % (maxW - minW) + minW;
    int h = qrand() % (maxH - minH) + minH;
    int maxX = scalar - w;
    int maxY = scalar - h;
    int x = qrand() % maxX;
    int y = qrand() % maxY;
    return QRectF(x/scalar, y/scalar, w/scalar, h/scalar);
}

void CColorFormsPicture::genRandom(int layers)
{
    clearColorForms();
    CRectColorForm *bgForm = new CRectColorForm(QRectF(0.0, 0.0, 1.0, 1.0), rndColor());
    m_colorForms.append(bgForm);
    int scalar = 1000;
    for(int i = 0; i < layers; i++) {
        int min = scalar/(i + 2);
        int max = scalar/(i + 1);
        int formCount = qrand() % 10 + 5;
        for(int f = 0; f < formCount; ++f) {
            QRectF formRect = rndRect(min, min, max, max, scalar);
            qreal angle = qrand() % 360;
            m_colorForms.append(new CRectColorForm(formRect, rndColor(), angle));
        }
    }
    update();
}

