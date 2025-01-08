#include "LoggingCallbacks.h"
#include <iostream>
#include <GLFW/glfw3.h> // For GLFW_KEY_* constants

void LoggingCallbacks::keyCallback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        std::cout << "called back" << std::endl;
    }
}
