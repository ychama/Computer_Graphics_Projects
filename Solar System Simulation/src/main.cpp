// #include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "UnitSphere.h"
#include "CelestialBody.h"

// Radius in km
const float sunRadius = 696340.0f;
const float earthRadius = 6371.0f;
const float moonRadius = 1737.4f;

const float relativeSunScale = 0.3f;

// Distance from parent (in km)
const float distFromSunToEarth = 125.52e6;
const float distFromEarthToMoon = 75e6;

// Axial tilts (in radians)
const float sunAxialTilt = 0.10472;
const float earthAxialTilt = 0.408407;
const float moonAxialTilt = 0.0872665;

// Orbital Inclination
const float earthOrbitalInclination = 0.523599;
const float moonOrbitalInclination = 0.523599;

// Orbital Rotation Speed
const float earthOrbitialRotationSpeed = 0.3f;
const float moonOrbitialRotationSpeed = 3.0f;

class Callbacks : public CallbackInterface
{

public:
	Callbacks()
		: camera(glm::radians(45.f), glm::radians(45.f), 3.0), aspect(1.0f), rightMouseDown(false), mouseOldX(0.0), mouseOldY(0.0), isAnimating(false), incrementSpeed(false), decrementSpeed(false), reset(false)
	{
	}

	virtual void keyCallback(int key, int scancode, int action, int mods)
	{

		if (key == GLFW_KEY_R && action == GLFW_RELEASE)
		{
			reset = true;
			isAnimating = false;
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		{
			isAnimating = !isAnimating;
		}

		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE && isAnimating)
		{
			incrementSpeed = true;
		}

		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE && isAnimating)
		{
			decrementSpeed = true;
		}
	}
	virtual void mouseButtonCallback(int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS)
				rightMouseDown = true;
			else if (action == GLFW_RELEASE)
				rightMouseDown = false;
		}
	}
	virtual void cursorPosCallback(double xpos, double ypos)
	{
		if (rightMouseDown)
		{
			camera.incrementTheta(ypos - mouseOldY);
			camera.incrementPhi(xpos - mouseOldX);
		}
		mouseOldX = xpos;
		mouseOldY = ypos;
	}
	virtual void scrollCallback(double xoffset, double yoffset)
	{
		camera.incrementR(yoffset);
	}
	virtual void windowSizeCallback(int width, int height)
	{
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width, height);
		aspect = float(width) / float(height);
	}

	void refreshStatuses()
	{
		incrementSpeed = false;
		decrementSpeed = false;
		reset = false;
	}

	bool getUserInput(int input)
	{
		if (input == GLFW_KEY_SPACE)
		{
			return isAnimating;
		}

		if (input == GLFW_KEY_RIGHT)
		{
			return incrementSpeed;
		}

		if (input == GLFW_KEY_LEFT)
		{
			return decrementSpeed;
		}

		if (input == GLFW_KEY_R)
		{
			return reset;
		}
	}

	void viewPipeline(ShaderProgram &sp)
	{
		glm::mat4 V = camera.getView();
		glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);

		GLint location = glGetUniformLocation(sp, "lightPosition");
		glm::vec3 light = glm::vec3(0.f, 0.f, 0.f);
		glUniform3fv(location, 1, glm::value_ptr(light));

		location = glGetUniformLocation(sp, "viewPos");
		glm::vec3 lightPos = camera.getPos();
		glUniform3fv(location, 1, glm::value_ptr(lightPos));

		GLint uniMat = glGetUniformLocation(sp, "V");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(V));
		uniMat = glGetUniformLocation(sp, "P");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(P));
	}

	Camera camera;

private:
	bool rightMouseDown;
	float aspect;
	double mouseOldX;
	double mouseOldY;

	bool isAnimating;

	bool incrementSpeed;
	bool decrementSpeed;

	bool reset;
};

int main()
{
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "Solar System Simulation");

	GLDebug::enable();

	// CALLBACKS
	auto callBacks = std::make_shared<Callbacks>();
	window.setCallbacks(callBacks);

	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	CelestialBody backdrop((sunRadius / 0.3f) * 8, "textures/8k_stars_milky_way.jpg", NULL, 0.f, 0.f, 0.f, 0.f);

	CelestialBody sun(sunRadius, "textures/8k_sun.jpg", NULL, 0.f, 0.f, sunAxialTilt, 0.f);

	CelestialBody earth(earthRadius * 60, "textures/8k_earth_daymap.jpg", &sun,
						distFromSunToEarth * 2, earthOrbitalInclination,
						earthAxialTilt, earthOrbitialRotationSpeed);

	CelestialBody moon(moonRadius * 40, "textures/8k_moon.jpg", &earth,
					   distFromEarthToMoon, moonOrbitalInclination,
					   moonAxialTilt, moonOrbitialRotationSpeed);

	bool prevIsAnimating = false;

	// RENDER LOOP
	while (!window.shouldClose())
	{

		callBacks->refreshStatuses();
		glfwPollEvents();

		if (callBacks->getUserInput(GLFW_KEY_R))
		{
			prevIsAnimating = false;
			backdrop.reset();
			sun.reset();
			earth.reset();
			moon.reset();
		}

		glEnable(GL_LINE_SMOOTH);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		shader.use();

		callBacks->viewPipeline(shader);

		if (callBacks->getUserInput(GLFW_KEY_SPACE) && callBacks->getUserInput(GLFW_KEY_SPACE) != prevIsAnimating)
		{
			prevIsAnimating = callBacks->getUserInput(GLFW_KEY_SPACE);
			sun.setIsAnimating(prevIsAnimating);
			earth.setIsAnimating(prevIsAnimating);
			moon.setIsAnimating(prevIsAnimating);
		}
		else if (!callBacks->getUserInput(GLFW_KEY_SPACE) && callBacks->getUserInput(GLFW_KEY_SPACE) != prevIsAnimating)
		{
			prevIsAnimating = callBacks->getUserInput(GLFW_KEY_SPACE);
			sun.setIsAnimating(prevIsAnimating);
			earth.setIsAnimating(prevIsAnimating);
			moon.setIsAnimating(prevIsAnimating);
		}

		if (callBacks->getUserInput(GLFW_KEY_SPACE))
		{
			if (callBacks->getUserInput(GLFW_KEY_RIGHT))
			{
				sun.setAxialRotationAngleIncrement(0.01f, .5f);
				earth.setAxialRotationAngleIncrement(0.01f, .5f);
				moon.setAxialRotationAngleIncrement(0.01f, .5f);
			}

			else if (callBacks->getUserInput(GLFW_KEY_LEFT))
			{
				sun.setAxialRotationAngleIncrement(-0.01f, -.5f);
				earth.setAxialRotationAngleIncrement(-0.01f, -.5f);
				moon.setAxialRotationAngleIncrement(-0.01f, -.5f);
			}
		}

		backdrop.draw(shader);
		sun.draw(shader);
		earth.draw(shader);
		moon.draw(shader);

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
		window.swapBuffers();
	}
	glfwTerminate();
	return 0;
}
