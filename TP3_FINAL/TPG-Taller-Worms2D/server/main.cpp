#include <iostream>

#include <spdlog/spdlog.h>

#include "../common/config.h"
#include "../common/const.h"
#include "../common/liberror.h"
#include "../common/logger.h"

#include "server.h"

#define SERVNAME argv[1]

#define LOGNAME "server"
#define LOGFILE "logs/server.txt"


int main(int argc, char* argv[]) try {

    if (argc != 2) {
        std::cerr << "Bad program call. Expected " << argv[0] << " <port/servicename>\n";
        return ERROR_;
    }

    Config::commonNode = YAML::LoadFile(YAML_PATH "/common_const.yaml");
    Config::yamlNode = YAML::LoadFile(YAML_PATH "/server_const.yaml");
    Config::levelLayoutNode = YAML::LoadFile(YAML_PATH "/level-bars.yaml");
    Config::spawnLayoutNode = YAML::LoadFile(YAML_PATH "/spawn_points.yaml");

    Logger l(LOGNAME, LOGFILE);
    Server sv(SERVNAME);
    sv.run();
    spdlog::dump_backtrace();
    return SUCCESS_;

} catch (const LibError& e) {
    spdlog::get("server")->error("{:s}", e.what());
    spdlog::dump_backtrace();
    return ERROR_;

} catch (const std::exception& e) {
    spdlog::get("server")->error("{:s}", e.what());
    spdlog::dump_backtrace();
    return ERROR_;

} catch (...) {
    spdlog::get("server")->error("Error desconocido");
    spdlog::dump_backtrace();
    return ERROR_;
}
