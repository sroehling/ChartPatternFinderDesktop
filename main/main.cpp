#include "mainwindow.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;

    QObject::connect(&app, SIGNAL(aboutToQuit()), &mainWindow, SLOT(appExitCleanupHandler()));
    QObject::connect(&mainWindow, SIGNAL(quitApplication()), &app, SLOT(quit()));

    mainWindow.resize( 1024, 768 );
    mainWindow.show();

    return app.exec();
}
