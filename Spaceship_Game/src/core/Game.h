#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "../graphics/Window.h"
#include "../graphics/ShaderProgram.h"
#include "../geometry/GameGeometry.h"
#include "../core/GameObject.h"
#include "../core/GameCallbacks.h"

void setupGameObjects(GameObject &ship,
                      std::vector<GameObject> &diamonds,
                      std::shared_ptr<GameGeometry> shipGeometry,
                      std::shared_ptr<GameGeometry> diamondGeometry);

void updateShipGeom(GameObject &ship,
                    const glm::vec3 &mousePos,
                    bool isWPressed,
                    bool isSPressed);

void gameLogic(GameObject &ship,
               std::vector<GameObject> &diamonds,
               int &score);

class Game
{
public:
    // Only the DECLARATION here, no inline definition:
    Game(std::shared_ptr<GameCallbacks> inputManager,
         std::shared_ptr<GameGeometry> shipGeometry,
         std::shared_ptr<GameGeometry> diamondGeometry,
         ShaderProgram &shaderRef,
         int windowWidth,
         int windowHeight);

    void reset();
    void update();
    void render(Window &window);

    int getScore() const { return score; }

private:
    std::shared_ptr<GameCallbacks> inputManager;
    std::shared_ptr<GameGeometry> shipGeom;
    std::shared_ptr<GameGeometry> diamondGeom;
    ShaderProgram &shader;

    GameObject ship; // no default constructor => must init in .cpp
    std::vector<GameObject> diamonds;

    int score;
    int windowWidth;
    int windowHeight;

    GLint transformLoc;
};
