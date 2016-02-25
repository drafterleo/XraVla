#ifndef CEDITORSFACTORY_H
#define CEDITORSFACTORY_H

#include "cw_abstractedit.h"
#include "cw_figuresedit.h"
#include "cw_colormatrixedit.h"

#include <QMap>

class CAbstractEditCreator
{
public:
    virtual CAbstractEdit *create() const = 0;
};

template <class C>
class CEditCreator: public CAbstractEditCreator
{
public:
    CAbstractEdit *create() const { return new C(); }
};


class CEditFactory
{
protected:
    typedef QMap<QString, CAbstractEditCreator *> TFactoryMap;
    TFactoryMap factoryMap;

    void registerEditClasses()
    {
        addEditClass <CFiguresEdit>("CFiguresEdit");
        addEditClass <CFiguresEdit>("CFiguresPixra");
        addEditClass <CColorMatrixEdit>("CColorMatrixEdit");
        addEditClass <CColorMatrixEdit>("CColorMatrixPixra");
    }

public:
    CEditFactory()
    {
        registerEditClasses();
    }

    template <class C>
    void addEditClass(const QString & className)
    {
        factoryMap[className] = new CEditCreator<C>();
    }

    CAbstractEdit * createEditor(const QString & className)
    {
        if (factoryMap.contains(className))
        {
            return factoryMap[className]->create();
        }

        return 0;
    }

};

extern CEditFactory editFactoryInstance;


#endif // CEDITORSFACTORY_H
