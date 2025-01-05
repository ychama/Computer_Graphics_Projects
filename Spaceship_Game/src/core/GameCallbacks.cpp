#include "GameCallbacks.h"
#include <GLFW/glfw3.h>

GameCallbacks::GameCallbacks(ShaderProgram &shader, int screenWidth, int screenHeight)
    : shader(shader),
      screenDim(screenWidth, screenHeight),
      isWPressed(false),
      isSPressed(false),
      restartGame(false) {}

void GameCallbacks::keyCallback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        shader.recompile();
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        restartGame = true;
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    {
        restartGame = false;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        isWPressed = true;
    }

    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        isWPressed = false;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        isSPressed = true;
    }

    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        isSPressed = false;
    }
}

bool GameCallbacks::isKeyPressed(int button)
{
    if (button == GLFW_KEY_W)
    {
        return isWPressed;
    }
    else if (button == GLFW_KEY_S)
    {
        return isSPressed;
    }
    else if (button == GLFW_KEY_ENTER)
    {
        return restartGame;
    }
    else
    {
        return false;
    }
}

glm::vec3 GameCallbacks::mouseGL()
{
    glm::vec2 startingVec(xScreenPos, yScreenPos);
    glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f, 0.5f);
    glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(screenDim);
    glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
    glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);
    return glm::vec3(final, 1.0f);
}

void GameCallbacks::cursorPosCallback(double xpos, double ypos)
{
    xScreenPos = xpos;
    yScreenPos = ypos;
}
