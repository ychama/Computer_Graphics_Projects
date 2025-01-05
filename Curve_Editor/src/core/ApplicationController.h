#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include "Camera.h"
#include "../graphics/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


// If you're on Windows or macOS with a case-sensitive or case-insensitive FS,
// verify that the actual file is named "camera.h" or "Camera.h" consistently.

class ApplicationController : public CallbackInterface
{
public:
    ApplicationController(int screenWidth, int screenHeight, Camera& camera);

    // These override your base CallbackInterface methods.
    // That means your Window class can call them when events happen:
    virtual void keyCallback(int key, int scancode, int action, int mods) override;
    virtual void mouseButtonCallback(int button, int action, int mods) override;
    virtual void cursorPosCallback(double xpos, double ypos) override;
    virtual void scrollCallback(double xoffset, double yoffset) override;
    virtual void windowSizeCallback(int width, int height) override;

    // Your own methods
    void refreshStatuses();
    glm::vec3 mouseGL();
    bool getUserInput(int input);

private:
    Camera& camera;

    glm::ivec2 screenDim;
    double xScreenPos;
    double yScreenPos;

    bool rightMousePressed;
    bool rightMouseReleased;
    bool enterPressed;
    bool rPressed;
    bool sceneOnePressed;
    bool sceneTwoPressed;
    bool sceneThreePressed;
    bool sceneFourPressed;
    bool tensorCurveSwitchPressed;
    bool wireFrameMode;
    bool threeDimMode;
};

#endif // APPLICATIONCONTROLLER_H
