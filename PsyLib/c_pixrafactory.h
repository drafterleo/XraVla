#ifndef C_PIXRAFACTORY_H
#define C_PIXRAFACTORY_H

#include "cw_abstractpixra.h"
#include "cw_figurespixra.h"
#include "cw_colormatrixpixra.h"
#include "cw_figuresedit.h"
#include "cw_colormatrixedit.h"

#include <QMap>

class CAbstractPixraCreator
{
public:
    virtual CAbstractPixra * create() const = 0;
};

template <class C>
class CPixraCreator: public CAbstractPixraCreator
{
public:
    CAbstractPixra * create() const { return new C(); }
};


class CPixraFactory
{
protected:
    typedef QMap<QString, CAbstractPixraCreator *> TFactoryMap;
    TFactoryMap factoryMap;

    void registerPixraClasses()
    {
        addPixraClass <CFiguresPixra>("CFiguresPixra");
        addPixraClass <CColorMatrixPixra>("CColorMatrixPixra");
    }

public:
    CPixraFactory()
    {
        registerPixraClasses();
    }

    template <class C>
    void addPixraClass(const QString & className)
    {
        factoryMap[className] = new CPixraCreator<C>();
    }

    CAbstractPixra * createPixra(const QString & className)
    {
        if (factoryMap.contains(className))
        {
            return factoryMap[className]->create();
        }

        return 0;
    }

};

extern CPixraFactory pixraFactoryInstance;

#endif // C_PIXRAFACTORY_H
