#ifndef MODIFILO_ABSTRAKTA_H
#define MODIFILO_ABSTRAKTA_H

#include <QWidget>
#include "simbolo_abstrakta.h"
#include "ilaro_abstrakta.h"

class KModifiloAbstrakta : public QWidget
{
    Q_OBJECT
public:
    KModifiloAbstrakta(QWidget *parent = 0);

    virtual KSimboloAbstrakta *simbolo() = 0;
    virtual void ingiSimbolo(KSimboloAbstrakta *ingotaSimbolo) = 0;
    virtual void ingiIlaro(KIlaroAbstrakta *ingotaIlaro) = 0;
};

#endif // MODIFILO_ABSTRAKTA_H
