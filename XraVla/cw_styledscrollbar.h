#pragma once

#include <QScrollBar>

class CStyledScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit CStyledScrollBar(QWidget *parent = 0);
};
