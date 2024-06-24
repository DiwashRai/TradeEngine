
#include "db_session.hpp"

bool db_session_server::handle_application(const unsigned seqnum,
                                           const FIX8::Message *&msg) {
    return enforce(seqnum, msg) || msg->process(_router);
}

bool db_session_client::handle_application(const unsigned seqnum,
                                           const FIX8::Message *&msg) {
    std::cout << "db_session_client::handle_application" << std::endl;
    return enforce(seqnum, msg) || msg->process(_router);
}