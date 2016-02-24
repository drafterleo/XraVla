#ifndef MODIFILO_DEFIGUROJ_H
#define MODIFILO_DEFIGUROJ_H

#include "modifilo_abstrakta.h"
#include "simbolo_defiguroj.h"
#include <QPointer>

class KModifiloDeFiguroj : public KModifiloAbstrakta
{
    Q_OBJECT
public:
    explicit KModifiloDeFiguroj(QWidget *parent = 0);

    /* virtualaj funkcioj */
    KSimboloAbstrakta *simbolo();
    void ingiSimbolo(KSimboloAbstrakta *ingotaSimbolo);
    void ingiIlaro(KIlaroAbstrakta *ingotaIlaro);

signals:

public slots:
    void aldoniRektangulon();
    void aldoniKvarangulon();
    void aldoniKvinangulon();
    void aldoniSesangulon();
    void eliminiFiguron();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QPointer <KSimboloDeFiguroj> m_simbolo; // sendangera(sekura) referenco

    int m_elektitaFiguro;
    int m_varmaFiguro;
    int m_kaptitaFiguro;
    int m_kaptitaAnkro;

    bool m_chuMousePremita;
    QPoint m_premitaPunkto;

    int m_ankroRadiuso;
};

#endif // MODIFILO_DEFIGUROJ_H
