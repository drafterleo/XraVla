#ifndef CFORMSPICTURE_H
#define CFORMSPICTURE_H

#include <QWidget>
#include <QVector>
#include "c_colorforms.h"

class CColorFormsPicture : public QWidget
{
    Q_OBJECT
public:
    explicit CColorFormsPicture(QWidget *parent = 0);
    ~CColorFormsPicture();
    void addForm(CAbstractColorForm *form);
    void genRandom(int layers = 5);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void clearColorForms();

private:
    QVector<CAbstractColorForm *> m_colorForms;
};

#endif // CFORMSPICTURE_H
