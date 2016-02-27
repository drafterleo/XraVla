#include <QFile>
#include <QTextStream>
#include "cw_styledmessagebox.h"

CStyledMessageBox::CStyledMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
    QFile inputFile(":/styles/MsgBox.style");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");
        QString styleStr = in.readAll();
        inputFile.close();

        setStyleSheet(styleStr);
    }
}

