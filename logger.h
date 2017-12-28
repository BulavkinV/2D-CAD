#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger
{
private:
    std::ofstream logfile;
public:
    Logger(const std::string&);
};

#endif // LOGGER_H
