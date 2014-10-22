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

macx: QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
macx: LIBS += -mmacosx-version-min=10.7 -stdlib=libc++
CONFIG += c++11

SOURCES += main.cpp\
    plot.cpp \
    griditem.cpp \
    legend.cpp \
    StockChartDateScaleDraw.cpp \
    StockChartPlotZoomer.cpp \
    StockChartPlotCurve.cpp \
    QDateHelper.cpp \
    MainWindow.cpp \
    PatternPlotCurve.cpp \
    PatternMatchTableView.cpp \
    InstrumentListTableView.cpp \
    PseudoTimeOHLCSample.cpp \
    BreakoutPlotMarker.cpp \
    InstrumentSelectionInfo.cpp \
    BreakdownPlotMarker.cpp \
    InstrumentList.cpp \
    PatternScanWorkerTask.cpp \
    StackedStockCharts.cpp \
    VolumePlot.cpp \
    VolumePlotCurve.cpp \
    VolumeYAxisScaleDraw.cpp \
    RegisterDialog.cpp

HEADERS  += \
    griditem.h \
    legend.h \
    plot.h \
    StockChartDateScaleDraw.h \
    StockChartPlotZoomer.h \
    StockChartPlotCurve.h \
    QDateHelper.h \
    MainWindow.h \
    PatternPlotCurve.h \
    PatternMatchTableView.h \
    InstrumentListTableView.h \
    PseudoTimeOHLCSample.h \
    BreakoutPlotMarker.h \
    InstrumentSelectionInfo.h \
    BreakdownPlotMarker.h \
    InstrumentList.h \
    PatternScanWorkerTask.h \
    StackedStockCharts.h \
    VolumePlot.h \
    VolumePlotCurve.h \
    VolumeYAxisScaleDraw.h \
    RegisterDialog.h


RESOURCES += icons.qrc

## PatternRecognitionLib is linked from the library built within the sub-module
macx: LIBS += -L$$PWD/lib/PatternRecognitionLib/build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/ -lPatternRecognitionLib
macx: PRE_TARGETDEPS += $$PWD/lib/PatternRecognitionLib/build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/libPatternRecognitionLib.a
macx: LIBS += -L$$PWD/lib/LicenseKey/build-LicenseKeyLib-Desktop_Qt_5_3_clang_64bit-Debug -lLicenseKeyLib
macx: PRE_TARGETDEPS += $$PWD/lib/LicenseKey/build-LicenseKeyLib-Desktop_Qt_5_3_clang_64bit-Debug/libLicenseKeyLib.a

win32: LIBS += -L$$PWD/lib/PatternRecognitionLib/build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Debug/debug/ -lPatternRecognitionLib
win32: PRE_TARGETDEPS += $$PWD/lib/PatternRecognitionLib/build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Debug/debug/libPatternRecognitionLib.a

## Link with pre-built version of Boost.
# IMPORTANT: To avoid link errors, the boost libraries must come *after* PatternRecognitionLib in the
# build sequence. gcc is especialy sensitive to this.
macx: DEFINES += BOOST_ALL_DYN_LINK
macx: DEFINES += BOOST_LOG_DYN_LINK
macx: INCLUDEPATH += /usr/local/boost156/include
macx: LIBS += -L/usr/local/boost156/lib -lboost_date_time-mt -lboost_log-mt -lboost_log_setup-mt -lboost_unit_test_framework-mt
macx: include ( /usr/local/qwt-6.1.0/features/qwt.prf )
win32: INCLUDEPATH += c:/boost_1_56_0
win32:LIBS += -L"C:/boost_1_56_0/stage/lib/" -lboost_date_time-mgw48-mt-1_56 -lboost_log-mgw48-mt-1_56 -lboost_log_setup-mgw48-mt-1_56
win32: include ( c:/qwt-6.1.1/qwt.prf )

INCLUDEPATH += $$PWD/lib/PatternRecognitionLib/src/chartSegment\
    $$PWD/lib/PatternRecognitionLib/src/chartSegmentList\
    $$PWD/lib/PatternRecognitionLib/src/date\
    $$PWD/lib/PatternRecognitionLib/src/math\
    $$PWD/lib/PatternRecognitionLib/src/patternMatch\
    $$PWD/lib/PatternRecognitionLib/src/patternMatchFilter\
    $$PWD/lib/PatternRecognitionLib/src/patternMatchValidator\
    $$PWD/lib/PatternRecognitionLib/src/patternScan\
    $$PWD/lib/PatternRecognitionLib/src/patternShape\
    $$PWD/lib/PatternRecognitionLib/src/quoteData\
    $$PWD/lib/LicenseKey/LicenseKeyLib

OTHER_FILES += \
    icons/chartpatternfinder.ico \
    ChartPatternFinder.rc \
    icons/cpf-loadquotes.png \
    icons/cpf-register.png \
    icons/cpf-reloadquotes.png \
    icons/cpf-upgrade.png \
    icons/cpf-help.png
