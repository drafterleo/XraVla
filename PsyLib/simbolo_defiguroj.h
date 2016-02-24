#ifndef SIMBOLO_DEFIGUROJ_H
#define SIMBOLO_DEFIGUROJ_H

#include "simbolo_abstrakta.h"
#include "c_figure.h"

class KSimboloDeFiguroj : public KSimboloAbstrakta
{
public:
    KSimboloDeFiguroj();

    void aldoniFiguro(CAbstractFigure * fg);
    void difiniGrando(QSize ingotaGrando);

    inline int kvantoDaFiguroj() {return m_figuroj.count();}
    inline const QSize & grando(void) {return m_grando;}
    CAbstractFigure* figuro(int indekso);


private:
    QSize m_grando;
    QVector <CAbstractFigure *> m_figuroj;
};

#endif // SIMBOLO_DEFIGUROJ_H
