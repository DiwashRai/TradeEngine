#ifndef DB_SESSIONWRAPPER_H
#define DB_SESSIONWRAPPER_H

#include <fix8/f8includes.hpp>
#include "ConsoleFileLogger.h"

namespace FIX8 {

template <typename T>
T Configuration::get_logflags(const std::string &tag,
                              const std::vector<std::string> &names,
                              const XmlElement *from,
                              Logger::LogPositions *positions) const {
    T flags;
    std::string flags_str;
    if (from && from->GetAttr(tag, flags_str)) {
        std::istringstream istr(flags_str);
        for (char extr[32]; !istr.get(extr, sizeof(extr), '|').fail();
             istr.ignore(1)) {
            std::string result(extr);
            const int evalue(flags.set(names, trim(result), true, true));
            if (positions && evalue >= 0) positions->push_back(evalue);
        }
    }

    return flags;
}

template <typename T>
class DBClientSession : public ClientSessionBase {
protected:
    sender_comp_id _sci;
    target_comp_id _tci;
    const SessionID _id;
    Logger *_log, *_plog;
    Persister *_persist;
    T *_session;
    Poco::Net::StreamSocket *_sock = nullptr;
    Poco::Net::SocketAddress _addr;
    ClientConnection *_cc = nullptr;
#ifdef FIX8_HAVE_OPENSSL
    PocoSslContext _ssl;
#endif

public:
    /// Ctor. Prepares session for connection as an initiator.
    DBClientSession(const F8MetaCntx &ctx, const std::string &conf_file,
                    const std::string &session_name,
                    bool init_con_later = false)
        : ClientSessionBase(ctx, conf_file, session_name),
          _sci(get_sender_comp_id(_ses)),
          _tci(get_target_comp_id(_ses)),
          _id(_ctx._beginStr, _sci, _tci),
          _log(dbcreate_logger(_ses, session_log, &_id)),
          _plog(dbcreate_logger(_ses, protocol_log, &_id)),
          _persist(create_persister(
              _ses, nullptr, this->_loginParameters._reset_sequence_numbers)),
          _session(new T(_ctx, _id, _persist, _log, _plog)),
          _addr(get_address(_ses))
#ifdef FIX8_HAVE_OPENSSL
          ,
          _ssl(get_ssl_context(_ses), true)
#endif
    {
        if (!init_con_later) {
#ifdef FIX8_HAVE_OPENSSL
            bool secured(_ssl.is_secure());
            _sock = secured ? new Poco::Net::SecureStreamSocket(_ssl._context)
                            : new Poco::Net::StreamSocket;
#else
            bool secured(false);
            _sock = new Poco::Net::StreamSocket;
#endif
            _cc = new ClientConnection(_sock, _addr, *_session,
                                       this->_loginParameters._hb_int,
                                       get_process_model(_ses), true, secured);
        }

        _session->set_login_parameters(this->_loginParameters);
        _session->set_session_config(this);
    }

    /// Dtor.
    ~DBClientSession() override {
        delete _persist;
        _persist = nullptr;
        delete _session;
        _session = nullptr;
        delete _log;
        _log = nullptr;
        delete _plog;
        _plog = nullptr;
    }

    /*! Get a pointer to the session
      \return the session pointer */
    T *session_ptr() override { return _session; }

    /*! Start the session - initiate the connection, logon and start
      heartbeating. \param wait if true wait till session finishes before
      returning \param send_seqnum if supplied, override the send login sequence
      number, set next send to seqnum+1 \param recv_seqnum if supplied, override
      the receive login sequence number, set next recv to seqnum+1 \param davi
      default appl version id (FIXT) */
    void start(bool wait, unsigned send_seqnum = 0, unsigned recv_seqnum = 0,
               const f8String davi = f8String()) override {
        _session->start(_cc, wait, send_seqnum, recv_seqnum, davi);
    }

    /*! Create a new logger object from a session entity.
      \param from xml entity to search
      \param ltype log type
      \param sid optional session id to build name from
      \return new logger or 0 if unable to create */
    Logger *dbcreate_logger(const XmlElement *from, const Logtype ltype,
                            const SessionID *sid = nullptr) const;

    /// Convenient scoped pointer for your session
    using DBClientSession_ptr = std::unique_ptr<DBClientSession<T>>;

    using session_type = T;
};

template <typename T>
Logger* DBClientSession<T>::dbcreate_logger(const XmlElement* from,
                                            const Logtype ltype,
                                            const SessionID* sid) const {
    std::string name;
    if (from_or_default(from,
                        ltype == session_log ? "session_log" : "protocol_log",
                        name)) {
        const XmlElement* which(find_group(g_loggers, name));
        if (which) {
            Logger::LogPositions positions;
            std::string type;

            if (which->GetAttr("type", type) &&
                ((type % "session" && ltype == session_log) ||
                 (type % "protocol" && ltype == protocol_log))) {
                std::string logname("logname_not_set.log"), levstr, delim(" ");
                which->FindAttrRef("filename", logname);
                trim(logname);
                if (which->GetAttr("delimiter", delim) &&
                    delim.size() > 2)  // "|" or "<>" or "{}" etc
                    throw ConfigurationError("invalid logging field delimiter");

                which->GetAttr("levels", levstr);
                const Logger::Levels levels(
                    levstr.empty() || levstr % "All"
                        ? Logger::Levels(Logger::All)
                    : levstr % "None"
                        ? Logger::Levels(Logger::None)
                        : get_logflags<Logger::Levels>(
                              "levels", Logger::_level_names, which));

                const Logger::LogFlags flags(
                    which->HasAttr("flags")
                        ? get_logflags<Logger::LogFlags>(
                              "flags", Logger::_bit_names, which, &positions)
                        : Logger::LogFlags(Logger::StdFlags));

                if (logname[0] == '|') {
#ifndef FIX8_HAVE_POPEN
                    throw ConfigurationError(
                        "popen not supported on your platform");
#endif
                    return new PipeLogger(logname, flags, levels, delim,
                                          positions);
                }

                RegMatch match;
                RegExp ipexp("^([^:]+):([0-9]+)$");
                if (ipexp.SearchString(match, logname, 3) == 3) {
                    f8String ip, port;
                    ipexp.SubExpr(match, logname, ip, 0, 1);
                    ipexp.SubExpr(match, logname, port, 0, 2);
                    return new BCLogger(ip, stoul(port), flags, levels, delim,
                                        positions);
                }

                get_logname(which, logname,
                            sid);  // only applies to file loggers
                if (flags & Logger::xml)
                    return new XmlFileLogger(logname, flags, levels, delim,
                                             positions,
                                             get_logfile_rotation(which));

                if (ltype == protocol_log)
                    return new FileLogger(logname, flags, levels, delim,
                                             positions,
                                             get_logfile_rotation(which));
                return new ConsoleFileLogger(logname, flags, levels, delim,
                                             positions,
                                             get_logfile_rotation(which));
            }
        }
    }

    return nullptr;

}


}  // namespace FIX8

#endif  // DB_SESSIONWRAPPER_H
