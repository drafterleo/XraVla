#ifndef CFORMSPICTURE_H
#define CFORMSPICTURE_H

#include <QWidget>
#include <QVector>
#include <QColor>
#include "c_colorforms.h"

class CColorFormsPicture : public QWidget
{
    Q_OBJECT
public:
    explicit CColorFormsPicture(QWidget *parent = 0);
    ~CColorFormsPicture();
    void addForm(CAbstractColorForm *form);
    void genRandom(int layers = 5);
    void rgbMax(int &r, int &g, int &b);
    void rgbMin(int &r, int &g, int &b);
    void lockColorBase();
    void shiftColors(int dR, int dG, int dB);
    void distortColors(int &distR, int &distG, int &distB);

    CColorFormsPicture& operator=(const CColorFormsPicture &other);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void clearColorForms();

private:
    QVector <CAbstractColorForm *> m_colorForms;
    QVector <QColor> m_colorBase;
};

#endif // CFORMSPICTURE_H
