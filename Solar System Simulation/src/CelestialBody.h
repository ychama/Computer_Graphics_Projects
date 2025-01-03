
#pragma once

#include "UnitSphere.h"
#include "ShaderProgram.h"

class CelestialBody
{

public:
	CelestialBody(float scale, std::string path, CelestialBody *parent = NULL,
				  float realDistanceFromParent = 0.0f, float orbitalInclination = 0.0f,
				  float axialTilt = 0.0f, float orbitalInclinationSpeed = 0.0f);

	GLsizei getUnitSphereSize();

	void generateTransformationMatrix();

	void draw(ShaderProgram &shader);

	void setAxialRotationAngleIncrement(float increment, float orbitalRotIncrement);

	void setIsAnimating(bool isAnimating);

	void reset();

private:
	UnitSphere sphere;
	CelestialBody *parent;
	glm::mat4 transformationMatrix;

	float scale;
	float distanceFromParent;
	float axialRotationAngle;
	float axialRotationAngleIncrement;
	float orbitalInclination;
	float orbitalInclinationSpeed;
	float axialTilt;
	float orbitalRotationIncrement;

	float totalAngle;

	float lastRecordedTime;

	bool isAnimating;

	glm::vec3 position;
};
