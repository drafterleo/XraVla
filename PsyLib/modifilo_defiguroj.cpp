#include "modifilo_defiguroj.h"
#include "ilaro_defiguroj.h"
#include <QPainter>
#include <QMouseEvent>

KModifiloDeFiguroj::KModifiloDeFiguroj(QWidget *parent) :
    KModifiloAbstrakta(parent)
{
    m_simbolo = 0;

    m_elektitaFiguro = -1;
    m_varmaFiguro = -1;
    m_kaptitaFiguro = -1;
    m_kaptitaAnkro = -1;
    m_ankroRadiuso = 5;
    m_chuMousePremita = false;

    setMouseTracking(true);
}

KSimboloAbstrakta *KModifiloDeFiguroj::simbolo()
{
    return m_simbolo;
}

void KModifiloDeFiguroj::ingiSimbolo(KSimboloAbstrakta *ingotaSimbolo)
{
    KSimboloDeFiguroj *simbolo = dynamic_cast<KSimboloDeFiguroj*>(ingotaSimbolo);
    if (simbolo) {
        m_simbolo = simbolo;
        m_simbolo->difiniGrando(this->size());
        update();
    }
}

void KModifiloDeFiguroj::ingiIlaro(KIlaroAbstrakta *ingotaIlaro)
{
    KIlaroDeFiguroj *ilaro = dynamic_cast<KIlaroDeFiguroj *>(ingotaIlaro);
    if (ilaro) {
        connect(ilaro, SIGNAL(necesasAldoniRektangulon()), SLOT(aldoniRektangulon()));
        connect(ilaro, SIGNAL(necesasAldoniKvarangulon()), SLOT(aldoniKvarangulon()));
        connect(ilaro, SIGNAL(necesasAldoniKvinangulon()), SLOT(aldoniKvinangulon()));
        connect(ilaro, SIGNAL(necesasAldoniSesangulon()), SLOT(aldoniSesangulon()));
    }
}


void KModifiloDeFiguroj::aldoniRektangulon()
{

}

void KModifiloDeFiguroj::aldoniKvarangulon()
{

}

void KModifiloDeFiguroj::aldoniKvinangulon()
{

}

void KModifiloDeFiguroj::aldoniSesangulon()
{

}

void KModifiloDeFiguroj::eliminiFiguron()
{

}


void KModifiloDeFiguroj::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(this->rect());

    if (m_simbolo) {
        for (int i = 0; i < m_simbolo->kvantoDaFiguroj(); ++i) {
            /* draw figure */
            painter.setBrush(QColor(100, 100, 100, 50));
            if (i == m_elektitaFiguro) {
                painter.setPen(QPen(QBrush(Qt::red), 2));
            } else {
                if (i == m_varmaFiguro)
                    painter.setPen(QPen(QBrush(Qt::green), 2));
                else
                    painter.setPen(QPen(QBrush(Qt::black), 2));
            }
            painter.drawPath(m_simbolo->figuro(i)->getPath());

            /* draw anchors */
            if (i == m_elektitaFiguro) {
                QVector <QPointF> iAnchors = m_simbolo->figuro(i)->getAnchors();
                painter.setBrush(QColor(100, 100, 100, 100));
                painter.setPen(QPen(QBrush(Qt::red), 2));
                for (int j = 0; j < iAnchors.count(); ++j) {
                    painter.drawEllipse(iAnchors.at(j), m_ankroRadiuso, m_ankroRadiuso);
                }
            }
        }
    }

    painter.end();
}

void KModifiloDeFiguroj::mousePressEvent(QMouseEvent *event)
{
    if (m_simbolo) {
        m_chuMousePremita = true;
        m_premitaPunkto = event->pos();

        if (m_elektitaFiguro >= 0) {
            m_kaptitaAnkro = m_simbolo->figuro(m_elektitaFiguro)->hitedAnchor(m_premitaPunkto);
            if (m_kaptitaAnkro >= 0) {
                this->update();
                return;
            }
            if (m_varmaFiguro < 0 && m_simbolo->figuro(m_elektitaFiguro)->getPath().contains(m_premitaPunkto)) {
                m_kaptitaFiguro = m_elektitaFiguro;
                return;
            }
        }

        int lastSpace = this->height() * this->width();
        m_kaptitaFiguro = -1;
        m_elektitaFiguro = -1;
        for (int i = 0; i < m_simbolo->kvantoDaFiguroj(); ++i) {
            if (m_simbolo->figuro(i)->getPath().contains(m_premitaPunkto)) {
                QRect pathRect = m_simbolo->figuro(i)->getPath().controlPointRect().toRect();
                int newSpace = pathRect.height() * pathRect.width();
                if (newSpace < lastSpace) {
                    lastSpace = newSpace;
                    m_elektitaFiguro = i;
                    m_kaptitaFiguro = i;
                }
            }
        }

        this->update();
    }
}

void KModifiloDeFiguroj::mouseMoveEvent(QMouseEvent *event)
{
    if (m_simbolo) {
        int lastHotFigure = m_varmaFiguro;
        m_varmaFiguro = -1;

        if (m_elektitaFiguro >= 0 && m_kaptitaAnkro >= 0) {
            qreal ax = qBound(0, event->pos().x(), this->width());
            qreal ay = qBound(0, event->pos().y(), this->height());
            m_simbolo->figuro(m_elektitaFiguro)->moveAnchor(m_kaptitaAnkro, QPointF(ax, ay));
            this->update();
            return;
        }

        if (m_chuMousePremita && m_kaptitaFiguro >= 0) {
            int dx = event->pos().x() - m_premitaPunkto.x();
            int dy = event->pos().y() - m_premitaPunkto.y();
            m_simbolo->figuro(m_kaptitaFiguro)->transform(QTransform().translate(dx, dy));
            if (!this->rect().contains(m_simbolo->figuro(m_kaptitaFiguro)->getPath().controlPointRect().toRect())) {
                m_simbolo->figuro(m_kaptitaFiguro)->transform(QTransform().translate(-dx, -dy));
            }
            this->update();
            m_premitaPunkto = event->pos();
            return;
        }

        int lastSpace = this->height() * this->width();
        for (int i = 0; i < m_simbolo->kvantoDaFiguroj(); ++i) {
            if (m_simbolo->figuro(i)->getPath().contains(event->pos() )) {
                QRect pathRect = m_simbolo->figuro(i)->getPath().controlPointRect().toRect();
                int newSpace = pathRect.height() * pathRect.width();
                if (newSpace < lastSpace) {
                    lastSpace = newSpace;
                    m_varmaFiguro = i;
                }
            }
        }

        if (m_varmaFiguro == m_elektitaFiguro)
            m_varmaFiguro = -1;
        if (m_varmaFiguro != lastHotFigure)
            this->update();
    }
}

void KModifiloDeFiguroj::mouseReleaseEvent(QMouseEvent *event)
{
    m_kaptitaAnkro = -1;
    m_kaptitaFiguro = -1;
    m_chuMousePremita = false;
}

