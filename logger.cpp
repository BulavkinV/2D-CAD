#include "logger.h"

Logger::Logger(const std::string& log_filename)
{
    logfile = std::ofstream(log_filename);
}
