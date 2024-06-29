
#include <fix8/f8includes.hpp>
#include <fix8/logger.hpp>

#include "ReplayClient.h"
#include "db_classes.hpp"
#include "db_session.hpp"
#include "db_sessionwrapper.h"

using namespace FIX8;

ReplayClient::ReplayClient(std::string config_file)
    : config_file_(std::move(config_file)) {
    GlobalLogger::set_global_filename("./log/ReplayClient.log");
}

void ReplayClient::run() const {
    DBClientSession<db_session_client> client(FIX4200_ctx(), config_file_,
                                            "ReplayClient");
    client.start(true);
}

