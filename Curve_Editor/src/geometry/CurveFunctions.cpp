#include "CurveFunctions.h"
#include <math.h>          // for M_PI, cos(), sin() if needed
#include <glm/vec3.hpp>    // ensures glm::vec3 is recognized
#define _USE_MATH_DEFINES
#include <math.h>




void drawBezierCurve(std::vector<glm::vec3>& controlPoints, CPU_Geometry& curvePts)
{
	// input to this is our ctrl prts and the degree

	int degree = controlPoints.size() - 1;

	std::vector<glm::vec3> intermediatePts = controlPoints;

	// Draw 100 pts
	for (float u = 0.0f; u < 1.0f; u += 0.01)
	{
		for (int i = 0; i < degree; i++)
		{
			for (int j = 0; j < degree - i; j++)
			{
				intermediatePts[j] = (1 - u) * intermediatePts[j] + u * intermediatePts[j + 1];
			}
		}
		curvePts.verts.push_back(intermediatePts[0]);
		curvePts.cols.push_back(glm::vec3{ 0.f, 0.0f, 1.0f });
	}
}

void drawBSpline(std::vector<glm::vec3>& controlPoints, CPU_Geometry& curvePts)
{
	// input to this is our ctrl prts and the degree

	std::vector<glm::vec3> intermediatePts = controlPoints;
	std::vector<glm::vec3> intermediateCurve;

	for (int i = 0; i < 5; i++)
	{

		int numberOfControlPts = intermediatePts.size();

		intermediateCurve.clear();
		// Special Mask at the Beginning

		intermediateCurve.push_back(intermediatePts[0]);
		intermediateCurve.push_back(0.5f * intermediatePts[0] + 0.5f * intermediatePts[1]);

		for (int i = 1; i < numberOfControlPts - 2; i++)
		{
			intermediateCurve.push_back(0.75f * intermediatePts[i] + 0.25f * intermediatePts[i + 1]);
			intermediateCurve.push_back(0.25f * intermediatePts[i] + 0.75f * intermediatePts[i + 1]);
		}

		// Special Mask at the end
		intermediateCurve.push_back(0.5f * intermediatePts[numberOfControlPts - 2] + 0.5f * intermediatePts[numberOfControlPts - 1]);
		intermediateCurve.push_back(intermediatePts[numberOfControlPts - 1]);

		intermediatePts = intermediateCurve;
	}

	curvePts.verts = intermediatePts;
	curvePts.cols.resize(intermediatePts.size(), glm::vec3{ 0.f, 0.0f, 1.0f });
}

void drawSurfaceOfRevolution(CPU_Geometry& surface, CPU_Geometry& bSpline)
{

	surface.verts.clear();
	surface.cols.clear();

	int numOfPts = 100;

	std::vector<std::vector<glm::vec3>> intermediatePts(bSpline.verts.size(), std::vector<glm::vec3>(numOfPts + 1));

	// i = outer loop
	// j = inner loop

	float v_increment = (2 * M_PI) / (numOfPts);

	for (int i = 0; i < bSpline.verts.size(); ++i)
	{
		int j = 0;
		for (float v = 0; v < (2 * M_PI) + v_increment; v += v_increment)
		{
			// Populate intermediate pts
			intermediatePts[i][j].x = bSpline.verts[i].x * cos(v);
			intermediatePts[i][j].y = bSpline.verts[i].y;
			intermediatePts[i][j].z = bSpline.verts[i].x * sin(v);

			if (i != 0)
			{

				if (j > 0)
				{
					surface.verts.push_back(intermediatePts[i][j]);
					surface.verts.push_back(intermediatePts[i][j - 1]);
					surface.verts.push_back(intermediatePts[i - 1][j]);
				}

				if (j < intermediatePts[i].size() - 1)
				{
					surface.verts.push_back(intermediatePts[i][j]);
					surface.verts.push_back(intermediatePts[i - 1][j]);
					surface.verts.push_back(intermediatePts[i - 1][j + 1]);
				}
			}

			j++;
		}
	}

	surface.cols.resize(surface.verts.size(), { 0.f, 0.f, 1.0f });
}

void drawTensor(CPU_Geometry& tensor, std::vector<std::vector<glm::vec3>>& bSpline)
{

	std::vector<std::vector<glm::vec3>> intermediateBSpline;
	std::vector<std::vector<glm::vec3>> tensorSurface;

	for (int i = 0; i < bSpline.size(); ++i)
	{
		CPU_Geometry curvePoints;

		drawBSpline(bSpline[i], curvePoints);

		intermediateBSpline.push_back(curvePoints.verts);
	}

	for (int j = 0; j < intermediateBSpline[0].size(); ++j)
	{

		std::vector<glm::vec3> intermediateColumn;

		for (int i = 0; i < intermediateBSpline.size(); ++i)
		{
			intermediateColumn.push_back(intermediateBSpline[i][j]);
		}

		CPU_Geometry curvePoints;
		drawBSpline(intermediateColumn, curvePoints);
		tensorSurface.push_back(curvePoints.verts);
	}

	for (int i = 0; i < tensorSurface.size(); ++i)
	{
		for (int j = 0; j < tensorSurface[i].size(); j++)
		{

			if (i != 0)
			{

				if (j > 0)
				{
					tensor.verts.push_back(tensorSurface[i][j]);
					tensor.verts.push_back(tensorSurface[i][j - 1]);
					tensor.verts.push_back(tensorSurface[i - 1][j]);
				}

				if (j < tensorSurface[i].size() - 1)
				{
					tensor.verts.push_back(tensorSurface[i][j]);
					tensor.verts.push_back(tensorSurface[i - 1][j]);
					tensor.verts.push_back(tensorSurface[i - 1][j + 1]);
				}
			}
		}
	}

	tensor.cols.resize(tensor.verts.size(), { 0.f, 0.f, 1.0f });
}

void getCurve(std::vector<glm::vec3>& controlPoints, CPU_Geometry& curvePts, CurveType curveType)
{

	// We switch on the enum instead of checking a string:
	switch (curveType)
	{
	case CurveType::Bezier:
		// use deCastljeu Algorithm
		if (controlPoints.size() > 2)
		{
			drawBezierCurve(controlPoints, curvePts);
		}
		break;

	case CurveType::BSpline:
		if (controlPoints.size() > 2)
		{
			drawBSpline(controlPoints, curvePts);
		}
		break;
	}
}

std::vector<std::vector<glm::vec3>> getTensorBSpline(std::string bSplineCurveType)
{
	// Example from assignment description
	if (bSplineCurveType == "1")
	{

		return std::vector<std::vector<glm::vec3>>(
			{ std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, -2.f}, glm::vec3{-1.f, 0.f, -2.f}, glm::vec3{0.f, 0.f, -2.f}, glm::vec3{1.f, 0.f, -2.f}, glm::vec3{2.f, 0.f, -2.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, -1.f}, glm::vec3{-1.f, 1.f, -1.f}, glm::vec3{0.f, 1.f, -1.f}, glm::vec3{1.f, 1.f, -1.f}, glm::vec3{2.f, 0.f, -1.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, 0.f}, glm::vec3{-1.f, 1.f, 0.f}, glm::vec3{0.f, -1.f, 0.f}, glm::vec3{1.f, 1.f, 0.f}, glm::vec3{2.f, 0.f, 0.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, 1.f}, glm::vec3{-1.f, 1.f, 1.f}, glm::vec3{0.f, 1.f, 1.f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec3{2.f, 0.f, 1.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, 2.f}, glm::vec3{-1.f, 0.f, 2.f}, glm::vec3{0.f, 0.f, 2.f}, glm::vec3{1.f, 0.f, 2.f}, glm::vec3{2.f, 0.f, 2.f}}) });
	}

	if (bSplineCurveType == "2")
	{

		return std::vector<std::vector<glm::vec3>>(
			{ std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, -2.f}, glm::vec3{0.f, 0.f, -2.f}, glm::vec3{1.f, 0.f, -2.f}, glm::vec3{2.f, 0.f, -2.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, -1.f}, glm::vec3{0.f, 1.f, -1.f}, glm::vec3{1.f, 1.f, -1.f}, glm::vec3{2.f, 0.f, -1.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, 0.f}, glm::vec3{0.f, -1.f, 0.f}, glm::vec3{1.f, 1.f, 0.f}, glm::vec3{2.f, 0.f, 0.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, 1.f}, glm::vec3{0.f, 1.f, 1.f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec3{2.f, 0.f, 1.f}}),
			 std::vector<glm::vec3>({glm::vec3{-2.f, 0.f, 2.f}, glm::vec3{0.f, 0.f, 2.f}, glm::vec3{1.f, 0.f, 2.f}, glm::vec3{2.f, 0.f, 2.f}}) });
	}
}
