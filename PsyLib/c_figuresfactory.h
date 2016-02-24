#ifndef C_FIGURESFACTORY_H
#define C_FIGURESFACTORY_H

#include "c_figure.h"
#include <QMap>

class CAbstractFigureCreator
{
public:
    virtual CAbstractFigure * create() const = 0;
};

template <class C>
class CFigureCreator: public CAbstractFigureCreator
{
public:
    CAbstractFigure * create() const { return new C(); }
};


class CFiguresFactory
{
protected:
    typedef QMap<QString, CAbstractFigureCreator *> TFactoryMap;
    TFactoryMap factoryMap;

    void registerFigureClasses()
    {
        addFigureClass <CPolygonFigure>("CPolygonFigure");
        addFigureClass <CRectangleFigure>("CRectangleFigure");
        addFigureClass <CEllipseFigure>("CEllipseFigure");
    }

public:
    CFiguresFactory()
    {
        registerFigureClasses();
    }

    template <class C>
    void addFigureClass(const QString & className)
    {
        factoryMap[className] = new CFigureCreator<C>();
    }

    CAbstractFigure * createFigure(const QString & className)
    {
        if (factoryMap.contains(className))
        {
            return factoryMap[className]->create();
        }

        return 0;
    }

};

extern CFiguresFactory figuresFactoryInstance;

#endif // C_FIGURESFACTORY_H
