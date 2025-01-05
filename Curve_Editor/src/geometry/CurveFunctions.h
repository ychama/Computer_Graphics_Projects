#ifndef CURVE_FUNCTIONS_H
#define CURVE_FUNCTIONS_H

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include "Geometry.h" // so CPU_Geometry is known
#include "CurveTypes.h" 


void drawBezierCurve(std::vector<glm::vec3>& controlPoints, CPU_Geometry& curvePts);
void drawBSpline(std::vector<glm::vec3>& controlPoints, CPU_Geometry& curvePts);
void drawSurfaceOfRevolution(CPU_Geometry& surface, CPU_Geometry& bSpline);
void drawTensor(CPU_Geometry& tensor, std::vector<std::vector<glm::vec3>>& bSpline);
void getCurve(std::vector<glm::vec3>& controlPoints, CPU_Geometry& curvePts, CurveType curveType);
std::vector<std::vector<glm::vec3>> getTensorBSpline(std::string bSplineCurveType);

#endif // CURVE_FUNCTIONS_H
