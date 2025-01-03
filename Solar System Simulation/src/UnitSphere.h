
#pragma once

#include "Geometry.h"
#include "Texture.h"
#include "string.h"

struct UnitSphere
{

public:
	UnitSphere(std::string path, GLint interpolation);
	void generateUnitSphere();
	void updateGPUGemoetry(GPU_Geometry &gpuGeom, std::vector<glm::vec3> const &vertices, std::vector<glm::vec2> const &texCoords);
	GPU_Geometry sphereGpuGeom;
	GLsizei m_size;
	Texture texture;

private:
	float radius;
};
