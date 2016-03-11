#include <QApplication>
#include <QTime>
#include "cf_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTime currTime = QTime::currentTime();
    qsrand(currTime.minute() + currTime.second() + currTime.msec());

    QString argFileName = QString();
    if (QApplication::arguments().count() > 1) {
        argFileName = QApplication::arguments().at(1);
    }

    CMainWindow *mainWindow = new CMainWindow(0, argFileName);
    mainWindow->setObjectName("MainWindow");
    mainWindow->show();

    return app.exec();
}
