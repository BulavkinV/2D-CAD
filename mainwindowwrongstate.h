#ifndef MAINWINDOWWRONGSTATE_H
#define MAINWINDOWWRONGSTATE_H

#include <exception>

class MainWindowWrongState : public std::exception
{
public:
    MainWindowWrongState();
};

#endif // MAINWINDOWWRONGSTATE_H
