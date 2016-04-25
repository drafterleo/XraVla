#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QMouseEvent>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

#include "cw_figuresedit.h"
#include "cw_colormatrixedit.h"
#include "cw_colormatrixpixra.h"
#include "cw_runepatternedit.h"
#include "color_wheel.hpp"
#include "cw_formspicture.h"

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

//        cmxEdit = new CColorMatrixEdit(this);
//            cmxEdit->setGeometry(150, 150, 450, 450);
//            cmxEdit->setColorMatrix(CColorMatrix(3, 3));
//            //cmxEdit->fillRandom();
//            //cmxEdit->setColor(1, 1, Qt::blue);

//        cmxPixra = new CColorMatrixPixra(this);
//        cmxPixra->setGeometry(10, 350, 60, 60);
//        cmxPixra->assign(cmxEdit->pixra());

        rpEdit = new CRunePatternEdit(this);
            rpEdit->setGeometry(310, 0, 100, 100);
            rpEdit->setPattern(CRunePattern(3, 3));
            rpEdit->genSolidLink(5, 1);

        protoColorPicture = new CColorFormsPicture(this);
        protoColorPicture->setGeometry(150, 150, 200, 200);

        cloneColorPicture = new CColorFormsPicture(this);
        cloneColorPicture->setGeometry(350, 350, 200, 200);

        int slX = 5;
        int slY = 350;
        int slW = 20;
        int slH = 200;
        redSlider = new QSlider(Qt::Vertical, this);
        redSlider->setGeometry(slX, slY, slW, slH);
        connect(redSlider, SIGNAL(valueChanged(int)), SLOT(sliderValueChanged(int)));
        greenSlider = new QSlider(Qt::Vertical, this);
        greenSlider->setGeometry(slX + slW + 10, slY, slW, slH);
        connect(greenSlider, SIGNAL(valueChanged(int)), SLOT(sliderValueChanged(int)));
        blueSlider = new QSlider(Qt::Vertical, this);
        blueSlider->setGeometry(slX + (slW + 10) * 2, slY, slW, slH);
        connect(blueSlider, SIGNAL(valueChanged(int)), SLOT(sliderValueChanged(int)));

        QPushButton *genBtn = new QPushButton("Gen", this);
            genBtn->move(10, 310);
        connect(genBtn, SIGNAL(clicked()), SLOT(genRandom()));

        infoLabel = new QLabel("info", this);
        infoLabel->setGeometry(10, 580, 200, 20);
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
//        cmxEdit->fillRandom();
//        cmxPixra->assign(cmxEdit->pixra());
        protoColorPicture->genRandom(5);
        *cloneColorPicture = *protoColorPicture;


        cloneColorPicture->distortColors(distR, distG, distB);
        cloneColorPicture->lockColorBase();

        int minR, minG, minB;
        int maxR, maxG, maxB;
        protoColorPicture->rgbMin(minR, minG, minB);
        protoColorPicture->rgbMax(maxR, maxG, maxB);
        redSlider->setMinimum((minR - maxR)/2);
        redSlider->setMaximum((maxR - minR)/2);
        redSlider->setValue(0);
        greenSlider->setMinimum((minG - maxG)/2);
        greenSlider->setMaximum((maxG - minG)/2);
        greenSlider->setValue(0);
        blueSlider->setMinimum((minB - maxB)/2);
        blueSlider->setMaximum((maxB - minB)/2);
        blueSlider->setValue(0);
    }

    void sliderValueChanged(int)
    {
        cloneColorPicture->shiftColors(redSlider->value(), greenSlider->value(), blueSlider->value());
        updateInfo();
    }

    void updateInfo()
    {
        int rShift = redSlider->value();
        int gShift = greenSlider->value();
        int bShift = blueSlider->value();
        QString infoStr = QString("R: %1, G: %2, B: %3").arg(rShift)
                                                        .arg(gShift)
                                                        .arg(bShift);

        infoLabel->setText(infoStr);
    }

private:
    CFiguresEdit       *figEdit;
    CColorMatrixEdit   *cmxEdit;
    CColorMatrixPixra  *cmxPixra;
    CRunePatternEdit   *rpEdit;
    CColorFormsPicture *protoColorPicture;
    CColorFormsPicture *cloneColorPicture;

    QSlider *redSlider;
    QSlider *greenSlider;
    QSlider *blueSlider;
    int distR;
    int distG;
    int distB;

    QLabel *infoLabel;

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
