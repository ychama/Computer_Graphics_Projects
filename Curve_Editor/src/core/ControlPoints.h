#pragma once
#include <vector>
#include <glm/vec3.hpp>

class ControlPoints
{
public:
    ControlPoints();

    std::vector<glm::vec3> controlPoints;
    std::vector<glm::vec3> controlPointsColors;

    int selectedPoint;
    float overlapThreshold;

    int doesControlPointExist(const std::vector<glm::vec3>& controlPoints,
        const glm::vec3& newPoint);

    void addPoint(std::vector<glm::vec3>& controlPoints,
        std::vector<glm::vec3>& controlPointsColors,
        const glm::vec3& mousePosition);

    void removePoint();

    void selectPoint(std::vector<glm::vec3>& controlPointsColors,
        int newlySelectedPoint);

    void unselectPoint();

    void updatePoint(const glm::vec3& mousePosition);

    void pointLogic(std::vector<glm::vec3>& controlPoints,
        std::vector<glm::vec3>& controlPointsColors,
        const glm::vec3& mousePosition);

    void resetPoints();
};
