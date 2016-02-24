#ifndef CW_STYLEDCHECKBOX_H
#define CW_STYLEDCHECKBOX_H

#include <QCheckBox>

class CStyledCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CStyledCheckBox(QWidget *parent = 0);
    CStyledCheckBox(const QString & text, QWidget *parent = 0);
    
private:
    void loadStyle();
};

#endif // CW_STYLEDCHECKBOX_H
