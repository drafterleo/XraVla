#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QMouseEvent>

#include "color_wheel.hpp"

using namespace color_widgets;

class CWorkArea: public QWidget
{
    Q_OBJECT
public:
    CWorkArea(QWidget * parent = 0): QWidget(parent)
    {
        resize(300, 300);
        wheel = new ColorWheel(this);
        wheel->resize(400/3, 400/3);
        wheel->setColor("#55aa7f");
        connect(wheel, SIGNAL(colorChanged(QColor)), SLOT(colorChanged(QColor)));
    }

protected:
    virtual void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)

        QPainter painter(this);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(this->rect());

        painter.setBrush(QBrush(selectedColor));
        painter.drawRect(this->rect().adjusted(width() - 50, height() - 50, 0, 0));

        painter.end();
    }

protected slots:
    void colorChanged(const QColor & color)
    {
        selectedColor = color;
        update();
    }

private:
    ColorWheel *wheel;
    QColor selectedColor;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setGeometry(100, 100, 350, 350);
    CWorkArea *pathArea = new CWorkArea(window);
    pathArea->move(20, 20);

    window->show();

    return app.exec();
}

#include "main.moc"
