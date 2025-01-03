
#include "UnitSphere.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

UnitSphere::UnitSphere(std::string path, GLint interpolation) : texture(path, interpolation)
{
}

void UnitSphere::updateGPUGemoetry(GPU_Geometry &gpuGeom, std::vector<glm::vec3> const &vertices, std::vector<glm::vec2> const &texCoords)
{

	gpuGeom.bind();
	gpuGeom.setVerts(vertices);
	gpuGeom.setNormals(vertices);
	gpuGeom.setTexCoords(texCoords);
}

void UnitSphere::generateUnitSphere()
{

	std::vector<std::vector<glm::vec3>> points;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;

	int numOfPoints = 20;

	// Draw 100 points
	float step = (M_PI) / numOfPoints;

	// Create the points
	for (int i = 0; i < numOfPoints + 1; ++i)
	{
		float theta = i * step;
		points.push_back(std::vector<glm::vec3>());

		for (int j = 0; j < (2 * numOfPoints) + 1; ++j)
		{
			float phi = j * step;
			glm::vec3 point = glm::vec3(glm::sin(theta) * glm::sin(phi), glm::cos(theta), glm::cos(phi) * glm::sin(theta));
			points[i].push_back(point);
		}
	}

	// Draw the Points

	for (int i = 1; i < numOfPoints + 1; ++i)
	{
		float theta = i * step;

		for (int j = 0; j < (2 * numOfPoints) + 1; ++j)
		{
			float phi = j * step;

			if (j > 0)
			{
				vertices.push_back(points[i][j]);
				vertices.push_back(points[i][j - 1]);
				vertices.push_back(points[i - 1][j]);

				texCoords.push_back(glm::vec2(phi / (2 * M_PI), 1 - theta / M_PI));
				texCoords.push_back(glm::vec2((phi - step) / (2 * M_PI), 1 - theta / M_PI));
				texCoords.push_back(glm::vec2(phi / (2 * M_PI), 1 - (theta - step) / M_PI));
			}

			if (j < 2 * numOfPoints)
			{
				vertices.push_back(points[i][j]);
				vertices.push_back(points[i - 1][j]);
				vertices.push_back(points[i - 1][j + 1]);

				texCoords.push_back(glm::vec2(phi / (2 * M_PI), 1 - theta / M_PI));
				texCoords.push_back(glm::vec2((phi) / (2 * M_PI), 1 - (theta - step) / M_PI));
				texCoords.push_back(glm::vec2((phi + step) / (2 * M_PI), 1 - (theta - step) / M_PI));
			}
		}
	}

	this->m_size = vertices.size();

	updateGPUGemoetry(this->sphereGpuGeom, vertices, texCoords);
}
