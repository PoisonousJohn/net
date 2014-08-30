#include "Transport.h"
#include "Listener.h"

namespace poison { namespace net { namespace http {

    void Transport::addListener(Listener *listener) {
        listeners.emplace(listener);
    }

    void Transport::removeListener(Listener *listener) {
        listeners.erase(listener);
    }

    void Transport::onRequestComplete(const Response &resp) {
        for (auto listener : listeners) {
            listener->onRequestComplete(resp);
        }
    }
}}}
