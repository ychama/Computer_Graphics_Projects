// GameLogic.cpp
#include "GameLogic.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void updateShipGeom(GameObject &ship,
                    glm::vec3 const &mousePos, // must match exactly
                    bool isWPressed,
                    bool isSPressed)
{

    // Scale
    glm::mat4 scaleMatrix = glm::mat4(
        glm::vec4(ship.scale, 0, 0, 0),
        glm::vec4(0, ship.scale, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(0, 0, 0, 1));

    // Rotation
    glm::vec3 vector_v = glm::normalize(mousePos - ship.position);

    glm::mat4 currentRotationMatrix = glm::mat4(
        glm::vec4(cos(ship.theta), -sin(ship.theta), 0, 0),
        glm::vec4(sin(ship.theta), cos(ship.theta), 0, 0),
        glm::vec4(0, 0, 0, 0),
        glm::vec4(0, 0, 0, 1));

    glm::vec3 currentShipDirection = currentRotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);

    float magnitudeOfCrossProduct = vector_v.x * currentShipDirection.y - vector_v.y * currentShipDirection.x;

    float angle_of_rotation = atan2f(magnitudeOfCrossProduct, glm::dot(vector_v, currentShipDirection));

    float rotation_speed = 0.01;

    if (abs(angle_of_rotation) > rotation_speed)
    {
        if (angle_of_rotation > 0)
            ship.theta += rotation_speed;
        else
            ship.theta -= rotation_speed;
    }

    glm::mat4 rotationMatrix = glm::mat4(
        glm::vec4(cos(ship.theta), -sin(ship.theta), 0, 0),
        glm::vec4(sin(ship.theta), cos(ship.theta), 0, 0),
        glm::vec4(0, 0, 0, 0),
        glm::vec4(0, 0, 0, 1));

    // Translate

    glm::vec4 translation_vector(0.f, 1.f, 0.f, 1.f);
    float mouvmentSpeed = 0.005;

    if (isWPressed)
    {
        // do nothing
    }
    else if (isSPressed)
    {
        mouvmentSpeed = -mouvmentSpeed;
    }
    else
    {
        mouvmentSpeed = 0.0;
    }

    glm::vec3 vec3_translation_vector = glm::vec3(rotationMatrix * translation_vector);

    ship.position = ship.position + vec3_translation_vector * mouvmentSpeed;

    glm::mat4 translationMatrix = glm::mat4(
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(ship.position, 1.f));

    ship.transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void gameLogic(GameObject &ship, std::vector<GameObject> &diamonds, int &score)
{
    for (int i = 0; i < static_cast<int>(diamonds.size()); ++i)
    {
        glm::vec3 vector_v = ship.position - diamonds[i].position;

        if (glm::length(vector_v) <= ship.scale + diamonds[i].scale)
        {
            diamonds.erase(diamonds.begin() + i);
            ship.scale *= 1.5f;
            ++score;
        }
    }
}

void setupGameObjects(
    GameObject &ship,
    std::vector<GameObject> &diamonds,
    std::shared_ptr<GameGeometry> shipGeometry,
    std::shared_ptr<GameGeometry> diamondGeometry)
{
    diamonds.clear();

    GameObject new_ship(shipGeometry);
    ship = new_ship;

    glm::mat4 scaleMatrix(
        glm::vec4(ship.scale, 0, 0, 0),
        glm::vec4(0, ship.scale, 0, 0),
        glm::vec4(0, 0, 0, 0),
        glm::vec4(0, 0, 0, 1));

    ship.transformationMatrix = scaleMatrix * ship.transformationMatrix;
    ship.geometry->ggeom.setVerts(ship.geometry->cgeom.verts);
    ship.geometry->ggeom.setTexCoords(ship.geometry->cgeom.texCoords);

    GameObject diamond1(diamondGeometry);
    diamond1.transformationMatrix = scaleMatrix * diamond1.transformationMatrix;

    GameObject diamond2(diamondGeometry);
    diamond2.transformationMatrix = scaleMatrix * diamond2.transformationMatrix;

    GameObject diamond3(diamondGeometry);
    diamond3.transformationMatrix = scaleMatrix * diamond3.transformationMatrix;

    GameObject diamond4(diamondGeometry);
    diamond4.transformationMatrix = scaleMatrix * diamond4.transformationMatrix;

    float diamond_abs_x_translation = 0.34f;
    float diamond_abs_y_translation = 0.51f;

    // top-left
    glm::mat4 diamond1_translationMatrix(
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(-(diamond_abs_x_translation), diamond_abs_y_translation, 0, 1));

    diamond1.position = glm::vec3((-diamond_abs_x_translation), diamond_abs_y_translation, 1.0f);
    diamond1.transformationMatrix = diamond1_translationMatrix * diamond1.transformationMatrix;

    // top-right
    glm::mat4 diamond2_translationMatrix(
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(diamond_abs_x_translation, diamond_abs_y_translation, 0, 1));

    diamond2.position = glm::vec3(diamond_abs_x_translation, diamond_abs_y_translation, 1.0f);
    diamond2.transformationMatrix = diamond2_translationMatrix * diamond2.transformationMatrix;

    // bottom-left
    glm::mat4 diamond3_translationMatrix(
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(-(diamond_abs_x_translation), -(diamond_abs_y_translation), 0, 1));

    diamond3.position = glm::vec3((-diamond_abs_x_translation), -(diamond_abs_y_translation), 1.0f);
    diamond3.transformationMatrix = diamond3_translationMatrix * diamond3.transformationMatrix;

    // bottom-right
    glm::mat4 diamond4_translationMatrix(
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(diamond_abs_x_translation, -(diamond_abs_y_translation), 0, 1));

    diamond4.position = glm::vec3(diamond_abs_x_translation, -(diamond_abs_y_translation), 1.0f);
    diamond4.transformationMatrix = diamond4_translationMatrix * diamond4.transformationMatrix;

    diamonds.push_back(diamond1);
    diamonds.push_back(diamond2);
    diamonds.push_back(diamond3);
    diamonds.push_back(diamond4);

    diamondGeometry->ggeom.setVerts(diamondGeometry->cgeom.verts);
    diamondGeometry->ggeom.setTexCoords(diamondGeometry->cgeom.texCoords);
}
