#ifndef GAME_CALLBACKS_H
#define GAME_CALLBACKS_H

#include "../graphics/Window.h"
#include "../graphics/ShaderProgram.h"
#include <glm/glm.hpp>

class GameCallbacks : public CallbackInterface
{
public:
    GameCallbacks(ShaderProgram &shader, int screenWidth, int screenHeight);

    virtual void keyCallback(int key, int scancode, int action, int mods) override;
    virtual void cursorPosCallback(double xpos, double ypos) override;

    bool isKeyPressed(int button);
    glm::vec3 mouseGL();

private:
    glm::ivec2 screenDim;
    double xScreenPos;
    double yScreenPos;
    bool isWPressed;
    bool isSPressed;
    bool restartGame;
    ShaderProgram &shader;
};

#endif // GAME_CALLBACKS_H
