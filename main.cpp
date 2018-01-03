#include <QApplication>
#include "mainwindow.h"
#include "gui_buttonarea.h"

#include <vector>
#include <string>

#include "logger.h"
#include "segment2p.h"

const std::string log_filename = "2dcad.log";

int main(int argc, char *argv[])
{
    Logger log(log_filename);

    QApplication app(argc, argv);

    MainWindow mw;
    GUI_ButtonArea ba(&mw);

    ba.show();
    mw.show();

    return app.exec();
}
