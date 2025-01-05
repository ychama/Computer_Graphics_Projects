#include "ControlPoints.h"
#include <glm/geometric.hpp> // for glm::length

ControlPoints::ControlPoints()
    : selectedPoint(-1), overlapThreshold(10.0f / 800.0f)
{
}

int ControlPoints::doesControlPointExist(const std::vector<glm::vec3>& controlPoints,
    const glm::vec3& newPoint)
{
    for (int i = 0; i < static_cast<int>(controlPoints.size()); i++)
    {
        glm::vec3 vector_v = newPoint - controlPoints[i];
        float length_of_v = glm::length(vector_v);
        if (length_of_v < overlapThreshold)
            return i;
    }
    return -1;
}

void ControlPoints::addPoint(std::vector<glm::vec3>& controlPoints,
    std::vector<glm::vec3>& controlPointsColors,
    const glm::vec3& mousePosition)
{
    controlPoints.push_back(mousePosition);
    controlPointsColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
}

void ControlPoints::removePoint()
{
    if (selectedPoint >= 0 && selectedPoint < static_cast<int>(controlPoints.size()))
    {
        controlPoints.erase(controlPoints.begin() + selectedPoint);
        controlPointsColors.erase(controlPointsColors.begin() + selectedPoint);
        selectedPoint = -1;
    }
}

void ControlPoints::selectPoint(std::vector<glm::vec3>& controlPointsColors,
    int newlySelectedPoint)
{
    controlPointsColors.at(newlySelectedPoint) = glm::vec3(0.0f, 1.0f, 0.0f);
    selectedPoint = newlySelectedPoint;
}

void ControlPoints::unselectPoint()
{
    if (selectedPoint != -1)
    {
        controlPointsColors.at(selectedPoint) = glm::vec3(1.0f, 0.0f, 0.0f);
        selectedPoint = -1;
    }
}

void ControlPoints::updatePoint(const glm::vec3& mousePosition)
{
    if (selectedPoint >= 0 && selectedPoint < static_cast<int>(controlPoints.size()))
    {
        controlPoints[selectedPoint] = mousePosition;
    }
}

void ControlPoints::pointLogic(std::vector<glm::vec3>& controlPoints,
    std::vector<glm::vec3>& controlPointsColors,
    const glm::vec3& mousePosition)
{
    int newlySelectedPoint = doesControlPointExist(controlPoints, mousePosition);
    if (newlySelectedPoint == -1)
    {
        addPoint(controlPoints, controlPointsColors, mousePosition);
    }
    else
    {
        selectPoint(controlPointsColors, newlySelectedPoint);
    }
}

void ControlPoints::resetPoints()
{
    controlPoints.clear();
    controlPointsColors.clear();
    selectedPoint = -1;
}
