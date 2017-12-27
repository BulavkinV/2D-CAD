#include "logger.h"

Logger::Logger(const std::string& log_filename)
{
    logfile = std::ifstream(log_filename);
}
