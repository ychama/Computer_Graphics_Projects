#include "ApplicationController.h"

// Include GLM core functionality before you use glm::vec2, glm::vec3, etc.
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp> // for vector subtraction, length, etc. if needed

#include <GLFW/glfw3.h>
#include <iostream>

// If you need 'SPEED' or other constants, ensure they are declared somewhere
// e.g., const float SPEED = 0.4f;

ApplicationController::ApplicationController(int screenWidth, int screenHeight, Camera& camera)
    : camera(camera)
    , screenDim(screenWidth, screenHeight)
    , rightMousePressed(false)
    , rightMouseReleased(false)
    , enterPressed(false)
    , rPressed(false)
    , sceneOnePressed(false)
    , sceneTwoPressed(false)
    , sceneThreePressed(false)
    , sceneFourPressed(false)
    , threeDimMode(false)
    , wireFrameMode(false)
    , tensorCurveSwitchPressed(false)
{
    xScreenPos = 0.0;
    yScreenPos = 0.0;
}

void ApplicationController::keyCallback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    {
        enterPressed = true;
    }

    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
        rPressed = true;
    }

    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Move camera forward
        camera.processKeyboardInput(FORWARD, SPEED);
    }

    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Move camera backward
        camera.processKeyboardInput(BACKWARD, SPEED);
    }

    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Move camera left
        camera.processKeyboardInput(LEFT, SPEED);
    }

    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Move camera right
        camera.processKeyboardInput(RIGHT, SPEED);
    }

    if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
    {
        sceneOnePressed = true;
    }

    if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
    {
        sceneTwoPressed = true;
    }

    if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
    {
        // Toggle sceneThreePressed
        sceneFourPressed = false;
        sceneThreePressed = !sceneThreePressed;
    }

    if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
    {
        // Toggle sceneFourPressed
        sceneThreePressed = false;
        sceneFourPressed = !sceneFourPressed;
    }

    if (key == GLFW_KEY_0 && action == GLFW_RELEASE)
    {
        // Toggle between 2D and 3D
        threeDimMode = !threeDimMode;
        if (threeDimMode)
        {
            // Reset scenes
            sceneThreePressed = false;
            sceneFourPressed = false;
            // Reset camera to default position
            camera.resetCamera();
        }
    }

    // Only allow toggling T if sceneFour is active
    if (key == GLFW_KEY_T && action == GLFW_RELEASE && sceneFourPressed)
    {
        tensorCurveSwitchPressed = !tensorCurveSwitchPressed;
    }

    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
    {
        wireFrameMode = !wireFrameMode;
    }
}

void ApplicationController::mouseButtonCallback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        rightMousePressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    {
        rightMouseReleased = true;
    }
}

void ApplicationController::cursorPosCallback(double xpos, double ypos)
{
    // Store old mouse pos in GL coords
    glm::vec3 prevMousePosGL = mouseGL();

    xScreenPos = xpos;
    yScreenPos = ypos;

    if (threeDimMode)
    {
        // If in 3D mode, rotate/pan the camera based on mouse movement
        glm::vec3 currMousePosGL = mouseGL();
        glm::vec3 deltaMousePosGL = currMousePosGL - prevMousePosGL;
        camera.processMouseInput(deltaMousePosGL.x, deltaMousePosGL.y);
    }
}

void ApplicationController::scrollCallback(double xoffset, double yoffset)
{
    // If you want to handle zoom with mouse wheel, do so here
    (void)xoffset;
    (void)yoffset;
}

void ApplicationController::windowSizeCallback(int width, int height)
{
    CallbackInterface::windowSizeCallback(width, height);
}

void ApplicationController::refreshStatuses()
{
    rightMousePressed = false;
    rightMouseReleased = false;
    enterPressed = false;
    rPressed = false;
    sceneOnePressed = false;
    sceneTwoPressed = false;
    // intentionally do NOT reset sceneThreePressed, sceneFourPressed, etc.
}

glm::vec3 ApplicationController::mouseGL()
{
    // Convert screen coords -> normalized device coords -> [-1, 1]
    glm::vec2 startingVec(static_cast<float>(xScreenPos), static_cast<float>(yScreenPos));
    glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f, 0.5f);

    // Divide by screenDim (also a glm::ivec2), but cast it to float vec2
    glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(
        static_cast<float>(screenDim.x),
        static_cast<float>(screenDim.y)
    );

    glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
    glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);

    // Return as 3D vector for convenience
    return glm::vec3(final, 1.0f);
}

bool ApplicationController::getUserInput(int input)
{
    switch (input)
    {
    case GLFW_MOUSE_BUTTON_1:
        return rightMousePressed;
    case -1:
        return rightMouseReleased;
    case GLFW_KEY_ENTER:
        return enterPressed;
    case GLFW_KEY_R:
        return rPressed;
    case GLFW_KEY_1:
        return sceneOnePressed;
    case GLFW_KEY_2:
        return sceneTwoPressed;
    case GLFW_KEY_3:
        return sceneThreePressed;
    case GLFW_KEY_4:
        return sceneFourPressed;
    case GLFW_KEY_0:
        return threeDimMode;
    case GLFW_KEY_TAB:
        return wireFrameMode;
    case GLFW_KEY_T:
        return tensorCurveSwitchPressed;
    default:
        return false;
    }
}
