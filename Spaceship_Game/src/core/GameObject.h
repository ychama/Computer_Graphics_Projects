#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../geometry/GameGeometry.h"
#include <glm/glm.hpp>
#include <memory>

struct GameObject
{
    GameObject(std::shared_ptr<GameGeometry> _g);

    std::shared_ptr<GameGeometry> geometry;

    glm::vec3 position;
    float theta; // Object's rotation
    float scale;
    glm::mat4 transformationMatrix;
};
#endif // GAME_OBJECT_H