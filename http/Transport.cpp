#include "Transport.h"
#include "Listener.h"

namespace poison { namespace net { namespace http {

    void Transport::addListener(Listener *listener) {
        listeners.emplace(listener);
    }

    void Transport::removeListener(Listener *listener) {
        listeners.erase(listener);
    }
    
    void Transport::doOnUpdate(const UpdateCallback&& callback) {
        Lock lock(m);
        updateCallbacks.emplace_back(std::move(callback));
    }

    void Transport::onRequestComplete(const Response &resp) {
        doOnUpdate([this, resp](){
            for (auto listener : listeners) {
                listener->onRequestComplete(resp);
            }
        });
    }
    
    void Transport::update() {
        Lock lock(m);
        
        for (auto& callback : updateCallbacks) {
            callback();
        }
        
        updateCallbacks.clear();
    }
}}}
