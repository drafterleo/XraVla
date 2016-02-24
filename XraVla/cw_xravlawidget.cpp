#include "cw_xravlawidget.h"
#include <QPainter>

CXravlaWidget::CXravlaWidget(QWidget *parent) :
    QWidget(parent)
{
    resize(400, 70);
    initStyles();

    wordEdit = new QLineEdit("toki", this);
    specEdit = new QTextEdit("toki", this);
    figuresPixra = new CFiguresPixra(this);

    wordEdit->setStyleSheet(m_wordStyle);
    specEdit->setStyleSheet(m_textStyle);

    specEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    wordEdit->setGeometry(75, 0, 200, 25);
    specEdit->setGeometry(75, 27, 200, 43);
    figuresPixra->setGeometry(0, 0, 70, 70);
}

void CXravlaWidget::assignItem(CXravlasteItem *item)
{
    if (item) {
        wordEdit->setText(item->word);
        specEdit->setText(item->spec);
        if (item->pixra)
            figuresPixra->assign(item->pixra);
        else
            figuresPixra->clear();
    } else {
        wordEdit->setText("???");
        specEdit->setText("???");
        figuresPixra->clear();
    }
}

void CXravlaWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);
    painter.drawRect(this->rect());
    painter.end();
}

void CXravlaWidget::initStyles()
{
    m_wordStyle  = "QLineEdit {"
                      "color: white;"
                      "background-color: transparent;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #466A7E;"
                   "}"
                   "QLineEdit:hover {"
                      "color: white;"
                      "background-color: transparent;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #538BC4;"
                   "}"
                   "QLineEdit:focus {"
                      "color: white;"
                      "background-color: transparent;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #538BC4;"
                   "}";

    m_textStyle = "QTextEdit {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #466A7E;"
                  "}"
                  "QTextEdit:hover {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #538BC4;"
                  "}"
                  "QTextEdit:focus {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #538BC4;"
                  "}";

}

