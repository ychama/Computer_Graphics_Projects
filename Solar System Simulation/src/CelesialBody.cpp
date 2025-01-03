
#include "CelestialBody.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

// Radius in km
const float sunRadius = 696340.0f;

CelestialBody::CelestialBody(float trueRadius, std::string path, CelestialBody *parent, float trueDistanceFromParent, float orbitalInclination, float axialTilt, float orbitalInclinationSpeed) : transformationMatrix(1.f), axialRotationAngle(M_PI / 2), sphere(path, GL_NEAREST)
{

	this->scale = (trueRadius / sunRadius) * 0.3f;
	this->distanceFromParent = (trueDistanceFromParent / sunRadius) * (0.3f / 100);
	this->parent = parent;
	this->axialRotationAngleIncrement = 0.01f;
	this->orbitalRotationIncrement = 1.f;
	this->orbitalInclination = orbitalInclination;
	this->axialTilt = axialTilt;
	this->position = glm::vec3(0.f, 0.f, 0.f);
	this->orbitalInclinationSpeed = orbitalInclinationSpeed;
	this->isAnimating = false;

	this->totalAngle = 0;
	this->lastRecordedTime = 0;

	// Draw the unit sphere
	sphere.generateUnitSphere();
}

GLsizei CelestialBody::getUnitSphereSize()
{

	return this->sphere.m_size;
}

void CelestialBody::setAxialRotationAngleIncrement(float axialRotIncrement, float orbitalRotIncrement)
{

	float temp = axialRotationAngleIncrement + axialRotIncrement;
	if (temp >= 0.01f)
	{
		this->axialRotationAngleIncrement += axialRotIncrement;
	}

	float temp2 = this->orbitalRotationIncrement + orbitalRotIncrement;

	if (temp2 >= 1.f)
	{
		this->orbitalRotationIncrement += orbitalRotIncrement;
	}
}

void CelestialBody::reset()
{
	this->position = glm::vec3(0.f, 0.f, 0.f);
	this->axialRotationAngle = M_PI / 2;
	this->axialRotationAngleIncrement = 0.01f;
	this->orbitalRotationIncrement = 1.f;
	this->totalAngle = 0;
	this->lastRecordedTime = 0;
	this->isAnimating = false;
	transformationMatrix = glm::mat4(1.f);
}

void CelestialBody::setIsAnimating(bool isAnimating)
{
	this->isAnimating = isAnimating;
}

void CelestialBody::generateTransformationMatrix()
{
	this->transformationMatrix = glm::mat4(1.f);
	// Scale
	glm::mat4 scaleMatrix = glm::mat4(
		glm::vec4(this->scale, 0, 0, 0),
		glm::vec4(0, this->scale, 0, 0),
		glm::vec4(0, 0, this->scale, 0),
		glm::vec4(0, 0, 0, 1));

	// Axial Rotation
	if (isAnimating)
	{
		this->axialRotationAngle += axialRotationAngleIncrement;
	}

	glm::mat4 axialRotationMatrix = glm::mat4(
		glm::vec4(glm::cos(this->axialRotationAngle), 0, glm::sin(this->axialRotationAngle), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(-glm::sin(this->axialRotationAngle), 0, glm::cos(this->axialRotationAngle), 0),
		glm::vec4(0, 0, 0, 1));

	glm::mat4 translationMatrix(1.f);
	glm::mat4 orbitalRotationMatrix(1.f);

	glm::mat4 axialTilt = glm::mat4(
		glm::vec4(glm::cos(this->axialTilt), -glm::sin(this->axialTilt), 0, 0),
		glm::vec4(glm::sin(this->axialTilt), glm::cos(this->axialTilt), 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1));

	glm::mat4 translationMatrixToParent(1.f);

	if (parent != NULL)
	{

		// Translation

		float time = glfwGetTime();
		float timeDiff = time - this->lastRecordedTime;

		this->lastRecordedTime = time;

		timeDiff *= orbitalRotationIncrement;

		if (isAnimating)
		{
			this->totalAngle += timeDiff;
		}

		// Unit circle in x-z plane
		glm::vec3 unitCircle = glm::vec3(glm::cos(this->totalAngle * orbitalInclinationSpeed), 0.f, glm::sin(this->totalAngle * orbitalInclinationSpeed));

		translationMatrix = glm::translate(glm::mat4(1.0f), unitCircle * distanceFromParent);

		this->position = unitCircle * distanceFromParent;

		orbitalRotationMatrix = glm::mat4(
			glm::vec4(glm::cos(this->orbitalInclination), -glm::sin(this->orbitalInclination), 0, 0),
			glm::vec4(glm::sin(this->orbitalInclination), glm::cos(this->orbitalInclination), 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 0, 0, 1));

		glm::vec3 newPos(0.f);

		newPos.x = (this->position.x * glm::cos(this->orbitalInclination)) - (this->position.y * glm::sin(this->orbitalInclination));
		newPos.y = -1 * (this->position.x * glm::sin(this->orbitalInclination)) + (this->position.y * glm::cos(this->orbitalInclination));
		newPos.z = this->position.z;

		this->position = newPos;

		this->position += parent->position;

		translationMatrixToParent = glm::translate(glm::mat4(1.0f), parent->position);
	}

	else
	{
		glm::vec3 temp = glm::vec3(0.f, 0.f, 0.f);
		translationMatrix = glm::translate(glm::mat4(1.0f), temp);
	}

	this->transformationMatrix = translationMatrixToParent * orbitalRotationMatrix * translationMatrix * axialTilt * axialRotationMatrix * scaleMatrix;
}

void CelestialBody::draw(ShaderProgram &shader)
{

	this->generateTransformationMatrix();

	bool isLightSrc = true;

	if (this->parent != NULL)
		isLightSrc = false;

	GLint isLightSource = glGetUniformLocation(shader, "isLightSource");

	glUniform1f(isLightSource, isLightSrc);

	GLint transformationMatrixShader = glGetUniformLocation(shader, "transformationMatrix");
	glUniformMatrix4fv(transformationMatrixShader, 1, GL_FALSE, &transformationMatrix[0][0]);

	sphere.texture.bind();

	glDrawArrays(GL_TRIANGLES, 0, getUnitSphereSize());

	sphere.texture.unbind();
}
