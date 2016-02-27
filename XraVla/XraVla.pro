CONFIG += c++11

QT += xml widgets

INCLUDEPATH = ../PsyLib/

HEADERS += \
    cw_pagemanager.h \
    cw_pagebar.h \
    c_xravlastemodel.h \
    c_xravlasteitem.h \
    c_xravlastedelegate.h \
    ../PsyLib/cw_figurespixra.h \
    ../PsyLib/cw_figuresedit.h \
    ../PsyLib/cw_abstractpixra.h \
    ../PsyLib/cw_abstractedit.h \
    ../PsyLib/c_figure.h \
    cf_mainwindow.h \
    cp_vocabularypage.h \
    cp_testpage.h \
    cw_xravlasteview.h \
    cw_pushbutton.h \
    ../PsyLib/c_figuresfactory.h \
    ../PsyLib/c_pixrafactory.h \
    cp_pagewidget.h \
    cfr_abstracttestframe.h \
    cfr_choicetestframe.h \
    c_testresult.h \
    cw_testengine.h \
    cw_teststats.h \
    cw_protopixrapopup.h \
    cw_styledscrollbar.h \
    cw_styledcheckbox.h \
    ../PsyLib/cw_colormatrixedit.h \
    ../PsyLib/cw_colormatrixpixra.h \
    ../PsyLib/c_colormatrix.h \
    ../PsyLib/c_editorsfactory.h \
    ../PsyLib/color_utils.hpp \
    ../PsyLib/color_wheel.hpp \
    cw_styledmessagebox.h

SOURCES += \
    main.cpp \
    cw_pagemanager.cpp \
    cw_pagebar.cpp \
    c_xravlastemodel.cpp \
    c_xravlasteitem.cpp \
    c_xravlastedelegate.cpp \
    ../PsyLib/cw_figurespixra.cpp \
    ../PsyLib/cw_figuresedit.cpp \
    ../PsyLib/cw_abstractpixra.cpp \
    ../PsyLib/cw_abstractedit.cpp \
    ../PsyLib/c_figure.cpp \
    cf_mainwindow.cpp \
    cp_vocabularypage.cpp \
    cp_testpage.cpp \
    cw_xravlasteview.cpp \
    cw_pushbutton.cpp \
    ../PsyLib/c_figuresfactory.cpp \
    cp_pagewidget.cpp \
    cfr_abstracttestframe.cpp \
    cfr_choicetestframe.cpp \
    cw_testengine.cpp \
    cw_teststats.cpp \
    cw_protopixrapopup.cpp \
    ../PsyLib/c_pixrafactory.cpp \
    cw_styledscrollbar.cpp \
    cw_styledcheckbox.cpp \
    ../PsyLib/cw_colormatrixedit.cpp \
    ../PsyLib/cw_colormatrixpixra.cpp \
    ../PsyLib/c_colormatrix.cpp \
    ../PsyLib/c_editorsfactory.cpp \
    ../PsyLib/color_utils.cpp \
    ../PsyLib/color_wheel.cpp \
    cw_styledmessagebox.cpp

RESOURCES += \
    XraVla.qrc

