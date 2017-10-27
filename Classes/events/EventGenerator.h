#ifndef __EVENT_GENERATOR_H__
#define __EVENT_GENERATOR_H__

#include <vector>
#include <string>
#include <algorithm>
#include "IEventListener.h"

class EventGenerator {

public:

    void notifyAll(const std::string& message) {
        std::for_each(_listeners.begin(), _listeners.end(),
                      [&](IEventListener* item){item->notify(message);});
    }

    void addListener(IEventListener* listener) {
        _listeners.push_back(listener);
    }

private:

    std::vector<IEventListener*> _listeners;
};

#endif // __EVENT_GENERATOR_H__
