#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T10:57:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PatternRecognitionDesktop
TEMPLATE = app

# Configure compiling and linking against libc++ instead of libstdc++
# This is needed to ensure consistency for all components, including
# the boost library.
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
LIBS += -mmacosx-version-min=10.7 -stdlib=libc++
CONFIG += c++11


SOURCES += main.cpp\
    plot.cpp \
    griditem.cpp \
    legend.cpp \
    StockChartDateScaleDraw.cpp \
    StockChartPlotZoomer.cpp \
    StockChartPlotCurve.cpp \
    QDateHelper.cpp \
    DownTrianglePlotMarker.cpp \
    MainWindow.cpp

HEADERS  += \
    griditem.h \
    legend.h \
    plot.h \
    StockChartDateScaleDraw.h \
    StockChartPlotZoomer.h \
    StockChartPlotCurve.h \
    QDateHelper.h \
    DownTrianglePlotMarker.h \
    MainWindow.h


## PatternRecognitionLib is linked from the library built within the sub-module
macx: LIBS += -L$$PWD/lib/PatternRecognitionLib/build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/ -lPatternRecognitionLib

## Link with pre-built version of Boost.
DEFINES += BOOST_ALL_DYN_LINK
DEFINES += BOOST_LOG_DYN_LINK
macx: INCLUDEPATH += /usr/local/boost156/include
macx: LIBS += -L/usr/local/boost156/lib -lboost_date_time-mt -lboost_log-mt -lboost_log_setup-mt -lboost_unit_test_framework-mt

macx: include ( /usr/local/qwt-6.1.0/features/qwt.prf )
macx: INCLUDEPATH += /usr/local/include

macx: INCLUDEPATH += $$PWD/../PatternRecognitionLib/src/chartSegment\
    $$PWD/../PatternRecognitionLib/src/chartSegmentList\
    $$PWD/../PatternRecognitionLib/src/date\
    $$PWD/../PatternRecognitionLib/src/math\
    $$PWD/../PatternRecognitionLib/src/patternMatch\
    $$PWD/../PatternRecognitionLib/src/patternMatchFilter\
    $$PWD/../PatternRecognitionLib/src/patternMatchValidator\
    $$PWD/../PatternRecognitionLib/src/patternScan\
    $$PWD/../PatternRecognitionLib/src/quoteData
