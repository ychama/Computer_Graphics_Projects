// GameLogic.h
#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include "GameObject.h"
#include "GameGeometry.h"

// Free functions used for setting up objects, updating ship, collisions, etc.
void setupGameObjects(GameObject &ship,
                      std::vector<GameObject> &diamonds,
                      std::shared_ptr<GameGeometry> shipGeometry,
                      std::shared_ptr<GameGeometry> diamondGeometry);

void updateShipGeom(GameObject &ship,
                    glm::vec3 const &mousePos,
                    bool isWPressed,
                    bool isSPressed);

void gameLogic(GameObject &ship,
               std::vector<GameObject> &diamonds,
               int &score);
