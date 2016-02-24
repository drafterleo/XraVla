#ifndef SIMBOLO_ABSTRAKTA_H
#define SIMBOLO_ABSTRAKTA_H

#include <QObject>

class KSimboloAbstrakta: public QObject
{
    Q_OBJECT
public:
    KSimboloAbstrakta(QObject *parent = 0);
};

#endif // SIMBOLO_ABSTRAKTA_H
