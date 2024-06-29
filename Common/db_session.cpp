
#include "db_session.hpp"
#include "db_classes.hpp"

bool db_session_server::handle_application(const unsigned seqnum,
                                           const FIX8::Message *&msg) {
    return enforce(seqnum, msg) || msg->process(_router);
}

void db_session_server::state_change(const FIX8::States::SessionStates before,
                                     const FIX8::States::SessionStates after) {
    loginfo(get_session_state_string(before) + " => " +
            get_session_state_string(after));
}

FIX8::Message *db_session_client::generate_logon(
    const unsigned heartbeat_interval, const f8String davi) {
    auto msg = static_cast<FIX8::FIX4200::Logon *>(
        Session::generate_logon(heartbeat_interval, davi));
    *msg << new FIX8::FIX4200::Username("User")
         << new FIX8::FIX4200::Password("Password");
    return msg;
}
bool db_session_client::handle_application(const unsigned seqnum,
                                           const FIX8::Message *&msg) {
    return enforce(seqnum, msg) || msg->process(_router);
}

void db_session_client::state_change(const FIX8::States::SessionStates before,
                                     const FIX8::States::SessionStates after) {
    loginfo(get_session_state_string(before) + " => " +
            get_session_state_string(after));
}
