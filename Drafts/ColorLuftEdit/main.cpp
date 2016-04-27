#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QMouseEvent>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QtDebug>

#include "cw_formspicture.h"

class CWorkArea: public QWidget
{
    Q_OBJECT
public:
    CWorkArea(QWidget * parent = 0)
        : QWidget(parent)
    {
        this->resize(610, 610);

        protoColorPicture = new CColorFormsPicture(this);
        protoColorPicture->setGeometry(150, 150, 200, 200);
        protoColorPicture->show();

        cloneColorPicture = new CColorFormsPicture(this);
        cloneColorPicture->setGeometry(350, 350, 200, 200);
        cloneColorPicture->show();

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
        protoColorPicture->genRandom();
        *cloneColorPicture = *protoColorPicture;

        distR = qrand() % 200 - 100;
        distG = qrand() % 200 - 100;
        distB = qrand() % 200 - 100;
        cloneColorPicture->shiftColors(distR, distG, distB);
        //qDebug() << distR << distG << distB;

        redSlider->setMinimum(-127);
        redSlider->setMaximum(+127);
        redSlider->setValue(0);
        greenSlider->setMinimum(-127);
        greenSlider->setMaximum(+127);
        greenSlider->setValue(0);
        blueSlider->setMinimum(-127);
        blueSlider->setMaximum(+127);
        blueSlider->setValue(0);
    }

    void sliderValueChanged(int)
    {
        cloneColorPicture->shiftColors(redSlider->value() + distR,
                                       greenSlider->value() + distG,
                                       blueSlider->value() + distB);
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
