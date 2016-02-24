#ifndef CW_XRAVLAWIDGET_H
#define CW_XRAVLAWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

#include "c_xravlasteitem.h"
#include "cw_figurespixra.h"

class CXravlaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CXravlaWidget(QWidget *parent = 0);

    void assignItem(CXravlasteItem *item);

    QLineEdit *wordEdit;
    QTextEdit *specEdit;
    CFiguresPixra *figuresPixra;

protected:
    void initStyles();

    void paintEvent(QPaintEvent *event);

private:
    QString m_wordStyle;
    QString m_textStyle;
};

#endif // CW_XRAVLAWIDGET_H
