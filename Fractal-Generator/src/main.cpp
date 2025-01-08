#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "./core/Application.h"

int main()
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to init GLFW");
	}
	// Create the application
	Application app(800, 800);

	// Run the application loop
	app.run();

	return 0;
}