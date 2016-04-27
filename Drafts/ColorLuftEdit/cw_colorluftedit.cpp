#include "cw_colorluftedit.h"

CColorLuftEdit::CColorLuftEdit(QWidget *parent)
    : CAbstractEdit(parent)
{
    viewMode = vmGen;
}

void CColorLuftEdit::clear()
{

}

CAbstractPixra *CColorLuftEdit::pixra(void)
{
    return 0;
}

bool CColorLuftEdit::assignPixra(CAbstractPixra *pixra)
{
    return true;
}

void CColorLuftEdit::randomize()
{

}


