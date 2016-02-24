#include "cw_styledcheckbox.h"
#include <QFile>
#include <QTextStream>

CStyledCheckBox::CStyledCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    loadStyle();
}

CStyledCheckBox::CStyledCheckBox(const QString & text, QWidget *parent)
    : QCheckBox(text, parent)
{
    loadStyle();
}

void CStyledCheckBox::loadStyle()
{
    QFile inputFile(":/styles/CheckBox.style");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");
        QString styleStr = in.readAll();
        inputFile.close();

        setStyleSheet(styleStr);
    }
}
