#include "simbolo_defiguroj.h"

KSimboloDeFiguroj::KSimboloDeFiguroj()
{
}

void KSimboloDeFiguroj::aldoniFiguro(CAbstractFigure * fg)
{
    if (fg) {
        m_figuroj.append(fg);
    }
}

void KSimboloDeFiguroj::difiniGrando(QSize ingotaGrando)
{
    m_grando = ingotaGrando;
}

CAbstractFigure* KSimboloDeFiguroj::figuro(int indekso)
{
    if (indekso >= 0 && indekso < m_figuroj.count()) {
        return m_figuroj.at(indekso);
    }
    return 0;
}

