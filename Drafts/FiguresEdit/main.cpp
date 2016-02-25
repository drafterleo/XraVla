#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QMouseEvent>
#include <QPushButton>

#include "cw_figuresedit.h"
#include "cw_colormatrixedit.h"
#include "cw_colormatrixpixra.h"
#include "cw_runepatternedit.h"
#include "color_wheel.hpp"

using namespace color_widgets;

class CWorkArea: public QWidget
{
    Q_OBJECT
public:
    CWorkArea(QWidget * parent = 0)
        : QWidget(parent)
    {
        this->resize(610, 610);

        figEdit = new CFiguresEdit(this);
            figEdit->setGeometry(0, 0, 150, 150);
            CPolygonFigure *pfig = new CPolygonFigure();
                pfig->addPoint(QPointF(10, 10));
                pfig->addPoint(QPointF(60, 15));
                pfig->addPoint(QPointF(55, 70));
                pfig->addPoint(QPointF(15, 60));
            figEdit->addFigure(pfig);
            //figEdit->addRandomPolygon(3, 100);
            //figEdit->addRandomPolygon(5, 120);
            CRectangleFigure *rfig = new CRectangleFigure(QRectF(10, 10, 80, 80));
            figEdit->addFigure(rfig);

        cmxEdit = new CColorMatrixEdit(this);
            cmxEdit->setGeometry(150, 150, 450, 450);
            cmxEdit->setColorMatrix(CColorMatrix(3, 3));
            //cmxEdit->fillRandom();
            //cmxEdit->setColor(1, 1, Qt::blue);

        cmxPixra = new CColorMatrixPixra(this);
        cmxPixra->setGeometry(10, 350, 60, 60);
        cmxPixra->assign(cmxEdit->pixra());

        rpEdit = new CRunePatternEdit(this);
            rpEdit->setGeometry(310, 0, 100, 100);
            rpEdit->setPattern(CRunePattern(3, 3));
            rpEdit->genSolidLink(5, 1);

        QPushButton *genBtn = new QPushButton("Gen", this);
            genBtn->move(10, 310);
        connect(genBtn, SIGNAL(clicked()), SLOT(genRandom()));
    }

protected:
    virtual void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)

        QPainter painter(this);

        painter.setPen(Qt::magenta);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(this->rect().adjusted(0, 0, -1, -1));

        painter.end();
    }

private slots:
    void genRandom()
    {
        rpEdit->genSolidLink(5, 1);
        cmxEdit->fillRandom();
        cmxPixra->assign(cmxEdit->pixra());
    }

private:
    CFiguresEdit      *figEdit;
    CColorMatrixEdit  *cmxEdit;
    CColorMatrixPixra *cmxPixra;
    CRunePatternEdit  *rpEdit;

};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setGeometry(100, 100, 650, 650);
    CWorkArea *workArea = new CWorkArea(window);
    workArea->move(10, 10);

    window->show();

    return app.exec();
}

#include "main.moc"
