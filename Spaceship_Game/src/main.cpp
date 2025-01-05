#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#include "./geometry/Geometry.h"
#include "./utils/GLDebug.h"
#include "./utils/Log.h"
#include "./graphics/ShaderProgram.h"
#include "./graphics/Shader.h"
#include "./graphics/Texture.h"
#include "./graphics/Window.h"
#include "./core/GameCallbacks.h"
#include "./geometry/GameGeometry.h"
#include "./core/GameObject.h"
#include "./core/Game.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

CPU_Geometry objGeom()
{
	CPU_Geometry retGeom;

	retGeom.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(-1.f, -1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(1.f, 1.f, 0.f));

	// texture coordinates
	retGeom.texCoords.push_back(glm::vec2(0.f, 1.f));
	retGeom.texCoords.push_back(glm::vec2(0.f, 0.f));
	retGeom.texCoords.push_back(glm::vec2(1.f, 0.f));
	retGeom.texCoords.push_back(glm::vec2(0.f, 1.f));
	retGeom.texCoords.push_back(glm::vec2(1.f, 0.f));
	retGeom.texCoords.push_back(glm::vec2(1.f, 1.f));
	return retGeom;
}

int main()
{
	Log::debug("Starting main");

	glfwInit();
	int windowWidth = 800;
	int windowHeight = 800;
	Window window(windowWidth, windowHeight, "Spaceship-Game");

	GLDebug::enable();

	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	auto inputManager = std::make_shared<GameCallbacks>(shader, windowWidth, windowHeight);
	window.setCallbacks(inputManager);

	// create geometry
	auto shipGeometry = std::make_shared<GameGeometry>(
		objGeom(), // your function
		"textures/ship.png",
		GL_NEAREST);

	auto diamondGeometry = std::make_shared<GameGeometry>(
		objGeom(),
		"textures/diamond.png",
		GL_NEAREST);

	// Create our Game
	Game myGame(inputManager, shipGeometry, diamondGeometry, shader, windowWidth, windowHeight);

	// MAIN LOOP
	while (!window.shouldClose())
	{
		glfwPollEvents();

		// 1) Update logic
		myGame.update();

		// 2) Render
		myGame.render(window);
	}

	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}
