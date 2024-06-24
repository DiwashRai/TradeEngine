#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <signal.h>

#include <atomic>

class SignalHandler {
public:
    static void initialise() {
        signal(SIGINT, singal_handler);
        signal(SIGTERM, singal_handler);
    }

    static bool termination_requested() {
        return term_received_.load(std::memory_order_relaxed);
    }

private:
    static inline std::atomic<bool> term_received_ = false;

    static void singal_handler(int signum) {
        switch (signum) {
            case SIGINT:
            case SIGTERM:
                term_received_.store(true, std::memory_order_relaxed);
                break;
            default:
                break;
        }
    }
};

#endif  // SIGNALHANDLER_H
