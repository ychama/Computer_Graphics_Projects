#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.4f;
const float SENSITIVITY = 20.0f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // Camera Attributes
    glm::vec3 positionVector;
    glm::vec3 frontVector;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processMouseInput(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void processKeyboardInput(Camera_Movement direction, float deltaTime);

    void updateCameraVectors();
    void resetCamera();
};

#endif // CAMERA_H
