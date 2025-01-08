#ifndef LOGGING_CALLBACKS_H
#define LOGGING_CALLBACKS_H
#include "../graphics/Window.h"

class LoggingCallbacks : public CallbackInterface
{
public:
    LoggingCallbacks() = default;
    void keyCallback(int key, int scancode, int action, int mods) override;
};

#endif // LOGGING_CALLBACKS_H