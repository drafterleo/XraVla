#ifndef ILARO_DEFIGUROJ_H
#define ILARO_DEFIGUROJ_H

#include "ilaro_abstrakta.h"
#include <QPushButton>

class KIlaroDeFiguroj : public KIlaroAbstrakta
{
    Q_OBJECT
public:
    explicit KIlaroDeFiguroj(QWidget *parent = 0);

signals:
    void necesasAldoniRektangulon();
    void necesasAldoniKvarangulon();
    void necesasAldoniKvinangulon();
    void necesasAldoniSesangulon();
    void necesasEliminiFiguron();

public slots:

private:
    QPushButton *m_rektanguloBtn;
    QPushButton *m_kvaranguloBtn;
    QPushButton *m_kvinanguloBtn;
    QPushButton *m_sesanguloBtn;
    QPushButton *m_eliminiFiguronBtn;
};

#endif // ILARO_DEFIGUROJ_H
