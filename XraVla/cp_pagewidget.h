#ifndef CP_PAGEWIDGET_H
#define CP_PAGEWIDGET_H

#include <QWidget>

class CPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPageWidget(QWidget *parent = 0);

    virtual void enterPage(void) {}
    virtual void exitPage(void) {}
};

#endif // CP_PAGEWIDGET_H
