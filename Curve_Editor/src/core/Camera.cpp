#include "Camera.h"

// ------------------------------------
// Constructor
// ------------------------------------
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : frontVector(glm::vec3(0.0f, 0.0f, 1.0f))
    , movementSpeed(SPEED)
    , mouseSensitivity(SENSITIVITY)
{
    this->positionVector = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

// ------------------------------------
// Returns the view matrix
// ------------------------------------
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(positionVector, positionVector + frontVector, up);
}

// ------------------------------------
// Returns a simple projection matrix
//  (You can adjust the aspect ratio, near/far planes, etc.)
// ------------------------------------
glm::mat4 Camera::getProjectionMatrix()
{
    // Example: aspect ratio = 800 / 800 = 1.0f
    return glm::perspective(glm::radians(ZOOM), 1.0f, 0.1f, 100.0f);
}

// ------------------------------------
// Process mouse movement
// ------------------------------------
void Camera::processMouseInput(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain the pitch
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        else if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update front, right and up Vectors using the updated Euler angles
    updateCameraVectors();
}

// ------------------------------------
// Process keyboard movement
// ------------------------------------
void Camera::processKeyboardInput(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        positionVector += frontVector * velocity;
    if (direction == BACKWARD)
        positionVector -= frontVector * velocity;
    if (direction == LEFT)
        positionVector -= right * velocity;
    if (direction == RIGHT)
        positionVector += right * velocity;
}

// ------------------------------------
// Recalculates the frontVector, right, and up vectors
// ------------------------------------
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontVector = glm::normalize(newFront);

    // Also re-calculate the Right and Up vectors
    right = glm::normalize(glm::cross(frontVector, worldUp));
    up = glm::normalize(glm::cross(right, frontVector));
}

// ------------------------------------
// Reset camera to a default position
// ------------------------------------
void Camera::resetCamera()
{
    positionVector = glm::vec3(0.0f, 0.0f, 3.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = YAW;
    pitch = PITCH;
    frontVector = glm::vec3(0.0f, 0.0f, 1.0f);

    updateCameraVectors();
}
