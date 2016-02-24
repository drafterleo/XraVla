#ifndef CW_ABSTRACTEDIT_H
#define CW_ABSTRACTEDIT_H

#include <QWidget>
#include "cw_abstractpixra.h"

class CAbstractEdit : public QWidget
{
    Q_OBJECT
public:
    explicit CAbstractEdit(QWidget *parent = 0);

    virtual void clear() = 0;
    virtual void clearHIstory(void) {}
    virtual CAbstractPixra *pixra(void) = 0;
    virtual bool assignPixra(CAbstractPixra *pixra) = 0;
    virtual void randomize() = 0;


signals:
    void modified();

public slots:

};

#endif // CW_ABSTRACTEDIT_H
