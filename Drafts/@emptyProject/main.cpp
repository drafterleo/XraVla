#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QMouseEvent>

class CWorkArea: public QWidget
{
    Q_OBJECT
public:
    CWorkArea(QWidget * parent = 0): QWidget(parent)
    {
        resize(300, 300);
    }

protected:
    virtual void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)

        QPainter painter(this);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(this->rect());

        painter.end();
    }


private:
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
