#ifndef CONSOLEFILELOGGER_H
#define CONSOLEFILELOGGER_H

#include <fix8/f8includes.hpp>

using namespace FIX8;

class ConsoleFileLogger : public Logger {
protected:
    std::string _pathname;
    unsigned _rotnum;

public:
    /*! Ctor.
        \param pathname pathname to log to
        \param flags ebitset flags
        \param levels ebitset levels
        \param delim field delimiter
        \param positions field positions
        \param rotnum number of logfile rotations to retain (default=5) */
    ConsoleFileLogger(const std::string& pathname, const LogFlags flags,
                     const Levels levels, const std::string delim = " ",
                     const LogPositions positions = LogPositions(),
                     const unsigned rotnum = rotation_default);

    /// Dtor.
    ~ConsoleFileLogger() override = default;

    void process_logline(LogElement* msg_ptr) override;

    /*! Perform logfile rotation
        \param force force the rotation (even if the file is set ti append)
        \return true on success */
    bool rotate(bool force = false) override;
};

#endif //CONSOLEFILELOGGER_H
