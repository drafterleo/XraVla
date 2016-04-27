#ifndef CCOLORLUFTEDIT_H
#define CCOLORLUFTEDIT_H

#include "cw_abstractedit.h"
#include "cw_formspicture.h"
#include "cw_pushbutton.h"
#include <QWidget>
#include <QStackedWidget>

class CGenCLEFrame;
class CFitCLEFrame;
class CInfoCLEFrame;

class CColorLuftEdit : public CAbstractEdit
{
    Q_OBJECT
public:
    CColorLuftEdit(QWidget *parent = 0);

    void clear();
    CAbstractPixra *pixra(void);
    bool assignPixra(CAbstractPixra *pixra);
    void randomize();
    void clearHIstory(void) {}

protected:
    enum VIEW_MODE {
        vmGen = 0,
        vmFit = 1,
        vmInfo = 2
    };

private:
    VIEW_MODE viewMode;

    QStackedWidget *frames;
    CGenCLEFrame   *genFrame;
    CFitCLEFrame   *fitFrame;
    CInfoCLEFrame  *infoFrame;
};


class CGenCLEFrame: public QWidget
{
    Q_OBJECT
public:
    CGenCLEFrame(QWidget *parent = 0)
        : QWidget(parent)
    {
        protoPic = new CColorFormsPicture(this);
        startBtn = new CPushButton(QPixmap(), this);
    }

    CColorFormsPicture *protoPic;
    CPushButton *startBtn;
};

class CFitCLEFrame: public QWidget
{
    Q_OBJECT
public:
    CFitCLEFrame(QWidget *parent = 0)
        : QWidget(parent)
    {
        fitPic = new CColorFormsPicture(this);
        stopBtn = new CPushButton(QPixmap(), this);
    }

    CColorFormsPicture *fitPic;
    CPushButton *stopBtn;
};


class CInfoCLEFrame: public QWidget
{
    Q_OBJECT
public:
    CInfoCLEFrame(QWidget *parent = 0)
        : QWidget(parent)
    {
        protoPic = new CColorFormsPicture(this);
        fitPic = new CColorFormsPicture(this);
        restartBtn = new CPushButton(QPixmap(), this);
    }

    CColorFormsPicture *protoPic;
    CColorFormsPicture *fitPic;
    CPushButton *restartBtn;
};



#endif // CCOLORLUFTEDIT_H
