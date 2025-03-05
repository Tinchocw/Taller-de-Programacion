#include "logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

Logger::Logger(const std::string& log_name, const std::string& logfile) {
    auto logger = spdlog::basic_logger_mt(log_name, logfile, true);
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::info);
    spdlog::set_pattern("%H:%M:%S.%e [%t] [%l] : %v");

    spdlog::enable_backtrace(20);
    // Que es el backtrace? Guarda los ultimos 20 mensajes del nivel [debug]. Si ocurre un error,
    // como este logger esta pensado RAII en su destructor se van a logear estos 20 ultimos al
    // ser llamado dump_backtrace().
    // Esto evita el spam de debug en el log
}

Logger::~Logger() { spdlog::dump_backtrace(); }
