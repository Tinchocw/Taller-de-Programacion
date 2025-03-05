#ifndef CLIENT_LOGGER_H
#define CLIENT_LOGGER_H

#include <string>

class Logger {
private:
public:
    /*
        @brief Configura al logger global de nombre log_name a dumpear los logs en logfile
    */
    explicit Logger(const std::string& log_name, const std::string& logfile);
    /*
        @brief Dumpea el backtrace del logger
    */
    ~Logger();
};

#endif
