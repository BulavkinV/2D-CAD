#include <QApplication>
#include "gui_mainwindow.h"
#include "gui_buttonarea.h"
#include "gui_statusbar.h"
#include "gui_centralwidget.h"
#include "gui_scene.h"

#include <geometryinterface.h>
#include <vector>
#include <string>
#include <QString>

#include "logger.h"
#include "segment2p.h"

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

const std::string log_filename = "2dcad.log";

int main(int argc, char *argv[])
{
    GeometryInterface interface;
    Logger log(log_filename);

    QApplication app(argc, argv);

    GUI_MainWindow window;
    GUI_ButtonArea buttons(&window);
    GUI_StatusBar status_bar;
    GUI_Scene scene(&window, &interface);
    GUI_CentralWidget central_wdidget(&window, &scene);

    window.setCentralWidget(&central_wdidget);
    window.setStatusBar(&status_bar);
    window.addToolBar(Qt::LeftToolBarArea, &buttons);
    window.show();

    return app.exec();
}
