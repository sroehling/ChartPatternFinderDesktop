#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T10:57:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Chart Pattern Finder"
TEMPLATE = app

win32:RC_FILE = ChartPatternFinder.rc

# Configure compiling and linking against libc++ instead of libstdc++
# This is needed to ensure consistency for all components, including
# the boost library.

macx: QMAKE_CXXFLAGS += -mmacosx-version-min=10.12 -std=c++11 -stdlib=libc++
macx: LIBS += -mmacosx-version-min=10.12 -stdlib=libc++
CONFIG += c++11

CONFIG(debug, debug|release) {
macx: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_12_2_clang_64bit-Debug
win32: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Debug\debug
} else {
macx: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_12_2_clang_64bit-Release
win32: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Release\release
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += NDEBUG
}

## PatternRecognitionLib is linked from the library built within the sub-module

LIBS += -L$$PWD/lib/ChartPatternRecognitionLib/$$PATTERNRECOGLIBDIR -lPatternRecognitionLib
PRE_TARGETDEPS += $$PWD/lib/ChartPatternRecognitionLib/$$PATTERNRECOGLIBDIR/libPatternRecognitionLib.a

## Link with pre-built version of Boost.
# IMPORTANT: To avoid link errors, the boost libraries must come *after* PatternRecognitionLib in the
# build sequence. gcc is especialy sensitive to this.
macx: DEFINES += BOOST_ALL_DYN_LINK
macx: DEFINES += BOOST_LOG_DYN_LINK
macx: INCLUDEPATH += /usr/local/include
macx: LIBS += -L/usr/local/lib -lboost_date_time-mt
macx: include ( /usr/local/opt/qwt/features/qwt.prf )
win32: INCLUDEPATH += c:/boost_1_56_0
win32:LIBS += -L"C:/boost_1_56_0/stage/lib/" -lboost_date_time-mgw48-mt-1_56
win32: include ( c:/qwt-6.1.1/qwt.prf )

SOURCES += \
    instrumentList/InstrumentSelectionInfo.cpp \
    instrumentList/InstrumentList.cpp \
    instrumentList/InstrumentListTableView.cpp \
    instrumentList/InstrumentListTask.cpp \
    instrumentList/ReadQuotesFromFile.cpp \
    instrumentList/InstrumentListWorker.cpp \
    instrumentList/ScanPatternMatches.cpp \
    main/main.cpp\
    main/MainWindow.cpp \
    main/StackedStockCharts.cpp \
    patternMatchList/PatternMatchTableView.cpp \
    patternPlot/PseudoTimeOHLCSample.cpp \
    patternPlot/StockChartDateScaleDraw.cpp \
    patternPlot/StockChartPlotZoomer.cpp \
    patternPlot/StockChartPlotCurve.cpp \
    patternPlot/PatternPlotCurve.cpp \
    patternPlot/BreakoutPlotMarker.cpp \
    patternPlot/PriceAndPatternPlot.cpp \
    patternPlot/BreakdownPlotMarker.cpp \
    util/QDateHelper.cpp \
    util/SettingsHelper.cpp \
    volumePlot/VolumePlot.cpp \
    volumePlot/VolumePlotCurve.cpp \
    volumePlot/VolumeYAxisScaleDraw.cpp \
    main/WelcomeDialog.cpp

HEADERS  += \
    instrumentList/InstrumentListTableView.h \
    instrumentList/InstrumentSelectionInfo.h \
    instrumentList/InstrumentList.h \
    instrumentList/InstrumentListTask.h \
    instrumentList/ReadQuotesFromFile.h \
    instrumentList/InstrumentListWorker.h \
    instrumentList/ScanPatternMatches.h \
    instrumentList/InstrumentListTypes.h \
    main/MainWindow.h \
    main/StackedStockCharts.h \
    patternMatchList/PatternMatchTableView.h \
    patternPlot/StockChartDateScaleDraw.h \
    patternPlot/StockChartPlotZoomer.h \
    patternPlot/StockChartPlotCurve.h \
    patternPlot/PatternPlotCurve.h \
    patternPlot/PriceAndPatternPlot.h \
    patternPlot/PseudoTimeOHLCSample.h \
    patternPlot/BreakoutPlotMarker.h \
    patternPlot/BreakdownPlotMarker.h \
    util/QDateHelper.h \
    util/SettingsHelper.h \
    volumePlot/VolumePlot.h \
    volumePlot/VolumePlotCurve.h \
    volumePlot/VolumeYAxisScaleDraw.h \
    main/WelcomeDialog.h

INCLUDEPATH += $$PWD/lib/PatternRecognitionLib/src/chartSegment\
    $$PWD/volumePlot\
    $$PWD/instrumentList\
    $$PWD/license\
    $$PWD/main\
    $$PWD/patternMatchList\
    $$PWD/patternPlot\
    $$PWD/util\
    $$PWD/lib/ChartPatternRecognitionLib/src/chartSegment\
    $$PWD/lib/ChartPatternRecognitionLib/src/chartSegmentList\
    $$PWD/lib/ChartPatternRecognitionLib/src/date\
    $$PWD/lib/ChartPatternRecognitionLib/src/math\
    $$PWD/lib/ChartPatternRecognitionLib/src/patternMatch\
    $$PWD/lib/ChartPatternRecognitionLib/src/patternMatchFilter\
    $$PWD/lib/ChartPatternRecognitionLib/src/patternMatchValidator\
    $$PWD/lib/ChartPatternRecognitionLib/src/patternScan\
    $$PWD/lib/ChartPatternRecognitionLib/src/patternShape\
    $$PWD/lib/ChartPatternRecognitionLib/src/quoteData

RESOURCES += icons.qrc

OTHER_FILES += \
    icons/chartpatternfinder.ico \
    ChartPatternFinder.rc \
    icons/cpf-loadquotes.png \
    icons/cpf-register.png \
    icons/cpf-reloadquotes.png \
    icons/cpf-upgrade.png \
    icons/cpf-help.png
