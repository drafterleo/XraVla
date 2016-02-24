#include <QFile>
#include <QTextStream>
#include "cw_styledscrollbar.h"

CStyledScrollBar::CStyledScrollBar(QWidget *parent) :
    QScrollBar(parent)
{
    setOrientation(Qt::Vertical);

    QFile inputFile(":/styles/ScrollBar.style");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");
        QString styleStr = in.readAll();
        inputFile.close();

        //qDebug() << styleStr;

        setStyleSheet(styleStr);
    }
}
