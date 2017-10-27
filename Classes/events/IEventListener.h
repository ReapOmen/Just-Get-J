#ifndef __I_EVENT_LISTENER_H__
#define __I_EVENT_LISTENER_H__

#include <string>

class IEventListener {

public:

    virtual void notify(const std::string& message) = 0;
};

#endif // __I_EVENT_LISTENER_H__
