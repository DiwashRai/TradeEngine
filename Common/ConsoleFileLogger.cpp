
#include "ConsoleFileLogger.h"

#include <fix8/f8includes.hpp>
#include <sstream>

ConsoleFileLogger::ConsoleFileLogger(const std::string& pathname,
                                     const LogFlags flags, const Levels levels,
                                     const std::string delim,
                                     const LogPositions positions,
                                     const unsigned rotnum)
    : Logger(flags, levels, delim, positions),
      _rotnum(rotnum)

{
    if (!pathname.empty()) {
        _pathname = pathname;
        rotate();
    }
}

void ConsoleFileLogger::process_logline(LogElement* msg_ptr) {
    std::ostringstream ostr;
    f8_scoped_spin_lock posguard(_log_spl);
    for (const auto pp : _positions) {
        std::ostringstream fostr;
        if (pp < start_controls) switch (pp) {
                case mstart: {
                    const Tickval tvs(msg_ptr->_when - _started);
                    fostr << std::setw(11) << std::right << std::setfill('0')
                          << (tvs.secs() * 1000 + tvs.msecs());
                } break;
                case sstart:
                    fostr << std::setw(8) << std::right << std::setfill('0')
                          << (msg_ptr->_when - _started).secs();
                    break;
                case sequence:
                    fostr << std::setw(7) << std::right << std::setfill('0');
                    if (_flags & direction)
                        fostr << (msg_ptr->_val ? ++_sequence : ++_osequence);
                    else
                        fostr << ++_sequence;
                    break;
                case thread:
                    fostr << get_thread_code(msg_ptr->_tid);
                    break;
                case location:
                    if (msg_ptr->_fileline) fostr << msg_ptr->_fileline;
                    break;
                case direction:
                    fostr << (msg_ptr->_val ? " in" : "out");
                    break;
                case timestamp:
                    fostr << msg_ptr->_when;
                    break;
                case minitimestamp:
                    fostr << GetTimeAsStringMini(&msg_ptr->_when);
                    break;
                case level:
                    fostr << _level_names[msg_ptr->_level];
                    break;
                default:
                    break;
            }

        if (!fostr.str().empty()) {
            if (_delim.size() > 1)
                ostr << _delim[0] << fostr.str() << _delim[1];
            else
                ostr << fostr.str() << _delim;
        }
    }
    posguard.release();

    f8_scoped_lock guard(_mutex);
    if (_delim.size() > 1)
        ostr << _delim[0] << msg_ptr->_str << _delim[1];
    else
        ostr << msg_ptr->_str;

    const std::string logline(ostr.str());
    get_stream() << logline;
    std::cout << logline;

    if (_flags & nolf) {
        get_stream().flush();
        std::cout.flush();
    } else {
        get_stream() << std::endl;
        std::cout << std::endl;
    }
}

bool ConsoleFileLogger::rotate(bool force) {
    f8_scoped_lock guard(_mutex);

    delete _ofs;

    std::string thislFile(_pathname), filepart, dirpart;
    split_path(thislFile, filepart, dirpart);
    if (!dirpart.empty() && !exist(dirpart))
        create_path(dirpart);

#ifdef HAVE_COMPRESSION
    if (_flags & compress)
        thislFile += ".gz";
#endif
    if (_rotnum > 0 && (!_flags.has(append) || force))
    {
        std::vector<std::string> rlst;
        rlst.push_back(thislFile);

        for (unsigned ii(0); ii < _rotnum && ii < max_rotation; ++ii)
        {
            std::ostringstream ostr;
            ostr << _pathname << '.' << (ii + 1);
            if (_flags & compress)
                ostr << ".gz";
            rlst.push_back(ostr.str());
        }

        for (unsigned ii(_rotnum); ii; --ii)
            rename (rlst[ii - 1].c_str(), rlst[ii].c_str());
    }

    const std::ios_base::openmode mode(_flags & append ? std::ios_base::out |
                                                             std::ios_base::app
                                                       : std::ios_base::out);
#ifdef HAVE_COMPRESSION
    if (_flags & compress)
        _ofs = new ogzstream(thislFile.c_str(), mode);
    else
#endif
        _ofs = new std::ofstream(thislFile.c_str(), mode);

    if (!_ofs || !*_ofs)
        throw LogfileException(thislFile);

    return true;
}
