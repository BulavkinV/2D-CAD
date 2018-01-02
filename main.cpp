#include "mainwindow.h"
#include <QApplication>

#include <string>

#include "logger.h"
#include "segment2p.h"

const std::string log_filename = "2dcad.log";

int main(int argc, char *argv[])
{
    Logger log(log_filename);

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
