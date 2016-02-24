#include <QApplication>
#include <QTime>
#include "cf_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTime currTime = QTime::currentTime();
    qsrand(currTime.minute() + currTime.second() + currTime.msec());

    CMainWindow *mainWindow = new CMainWindow;
    mainWindow->setObjectName("MainWindow");
    mainWindow->show();

    return app.exec();
}
