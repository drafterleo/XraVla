#ifndef CW_PROTOPIXRAPOPUP_H
#define CW_PROTOPIXRAPOPUP_H

#include <QWidget>
#include "cw_abstractpixra.h"
#include "c_pixrafactory.h"

class CProtoPixraPopup : public QWidget
{
    Q_OBJECT
public:
    explicit CProtoPixraPopup(QWidget *parent = 0);
    ~CProtoPixraPopup();

    void addProtoPixra(CAbstractPixra *pixra);

signals:
    void pixraSelected(CAbstractPixra *pixra);

protected:
    void updateRects();

    void paintEvent(QPaintEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    int m_pixraWidth;
    int m_pixraHeight;
    int m_span;

    int m_hotPixraIdx;
    int m_pressPixraIdx;

    QVector <CAbstractPixra *> m_protoPixras;
    QVector <QRect> m_rects;

};

#endif // CW_PROTOPIXRAPOPUP_H
