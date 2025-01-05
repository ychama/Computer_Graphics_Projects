// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>


#include "geometry/Geometry.h"
#include "geometry/CurveFunctions.h"
#include "geometry/CurveTypes.h"

#include "utils/GLDebug.h"
#include "utils/Log.h"

#include "graphics/ShaderProgram.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Window.h"

#include "core/Camera.h"
#include "core/ApplicationController.h"
#include "core/ControlPoints.h"


#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


#define _USE_MATH_DEFINES
#include <math.h>

enum class SceneMode
{
	TwoD,
	ThreeD
};



// We gave this code in one of the tutorials, so leaving it here too
void updateGPUGeometry(GPU_Geometry &gpuGeom, CPU_Geometry const &cpuGeom)
{
	gpuGeom.bind();
	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);
}

int main()
{
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "Curve Editor"); // can set callbacks at construction if desired

	window.cursorTwoD();

	// Initialize camera
	Camera camera;

	GLDebug::enable();

	// CALLBACKS
	auto application = std::make_shared<ApplicationController>(800, 800, camera);
	window.setCallbacks(application);

	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// The current CPU_Geometry and GPU_Geometry classes are defined in Geometry.h/Geometry.cpp 

	CPU_Geometry square;
	CPU_Geometry curvePts;
	ControlPoints cp;

	CPU_Geometry surface;
	CPU_Geometry tensor;

	GPU_Geometry pointsGPUGeom;
	GPU_Geometry curveGPUGeom;
	GPU_Geometry surfaceGPUGeom;
	GPU_Geometry tensorGPUGeom;
	
	SceneMode sceneMode = SceneMode::TwoD;  // start in 2D
	CurveType curveType = CurveType::Bezier;


	std::string prevTensor = "0";
	std::string currentTensor = "0";

	// In 2D you want view and projection matrix to be the identity
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	glPointSize(10.0f);


	// RENDER LOOP

	while (!window.shouldClose())
	{

		application->refreshStatuses();
		glfwPollEvents();

		// Get the curveType
		if (application->getUserInput(GLFW_KEY_1))
		{
			curveType = CurveType::Bezier;
			curvePts.verts.clear();
			curvePts.cols.clear();

			getCurve(cp.controlPoints, curvePts, curveType);

			updateGPUGeometry(curveGPUGeom, curvePts);
		}

		else if (application->getUserInput(GLFW_KEY_2))
		{
			curveType = CurveType::BSpline;
			curvePts.verts.clear();
			curvePts.cols.clear();

			getCurve(cp.controlPoints, curvePts, curveType);

			updateGPUGeometry(curveGPUGeom, curvePts);
		}


		// if KEY_0 is pressed => 3D mode, else => 2D mode

		if (application->getUserInput(GLFW_KEY_0))
		{
			sceneMode = SceneMode::ThreeD;
		}
		else
		{
			sceneMode = SceneMode::TwoD;
		}

		// Depending on sceneMode, do 2D or 3D
		switch (sceneMode)
		{
			// ------------------------------
			// 2D Mode
			// ------------------------------
		case SceneMode::TwoD:
		{
			window.cursorTwoD();
			viewMatrix = glm::mat4(1.0f);
			projectionMatrix = glm::mat4(1.0f);

			surface.verts.clear();
			surface.cols.clear();

			// R resets everything
			if (application->getUserInput(GLFW_KEY_R))
			{
				cp.resetPoints();

				square.verts.clear();
				square.cols.clear();

				curvePts.verts.clear();
				curvePts.cols.clear();

				updateGPUGeometry(pointsGPUGeom, square);
			}

			// Mouse Press => add or select a point
			if (application->getUserInput(GLFW_MOUSE_BUTTON_1))
			{
				glm::vec3 mousePos = application->mouseGL();
				cp.pointLogic(cp.controlPoints, cp.controlPointsColors, mousePos);

				square.verts = cp.controlPoints;
				square.cols = cp.controlPointsColors;

				curvePts.verts.clear();
				curvePts.cols.clear();

				getCurve(cp.controlPoints, curvePts, curveType);
				updateGPUGeometry(pointsGPUGeom, square);
				updateGPUGeometry(curveGPUGeom, curvePts);
			}
			// Mouse Release => unselect
			else if (application->getUserInput(-1))
			{
				cp.unselectPoint();
				square.verts = cp.controlPoints;
				square.cols = cp.controlPointsColors;

				curvePts.verts.clear();
				curvePts.cols.clear();

				getCurve(cp.controlPoints, curvePts, curveType);
				updateGPUGeometry(pointsGPUGeom, square);
				updateGPUGeometry(curveGPUGeom, curvePts);
			}

			// If there is a point selected and Enter is pressed => delete it
			if (application->getUserInput(GLFW_KEY_ENTER) && cp.selectedPoint != -1)
			{
				cp.removePoint();
				square.verts = cp.controlPoints;
				square.cols = cp.controlPointsColors;
				updateGPUGeometry(pointsGPUGeom, square);
			}
			// If a point is selected => move it with mouse
			else if (cp.selectedPoint != -1)
			{
				glm::vec3 mousePos = application->mouseGL();
				cp.updatePoint(mousePos);
				square.verts = cp.controlPoints;
				square.cols = cp.controlPointsColors;
				updateGPUGeometry(pointsGPUGeom, square);
			}
		}
		break;

		// ------------------------------
		// 3D Mode
		// ------------------------------
		case SceneMode::ThreeD:
		{
			window.cursorThreeD();
			viewMatrix = camera.getViewMatrix();
			projectionMatrix = camera.getProjectionMatrix();

			// KEY_3 => generate surface of revolution
			if (application->getUserInput(GLFW_KEY_3))
			{
				getCurve(cp.controlPoints, curvePts, CurveType::BSpline);
				drawSurfaceOfRevolution(surface, curvePts);
				updateGPUGeometry(surfaceGPUGeom, surface);
			}

			// T => set currentTensor = "1", else "2"
			if (application->getUserInput(GLFW_KEY_T))
			{
				currentTensor = "1";
			}
			else
			{
				currentTensor = "2";
			}

			// KEY_4 => draw tensor if changed
			if (application->getUserInput(GLFW_KEY_4))
			{
				if (prevTensor != currentTensor)
				{
					tensor.verts.clear();
					tensor.cols.clear();
					std::vector<std::vector<glm::vec3>> tensorBSpline = getTensorBSpline(currentTensor);
					drawTensor(tensor, tensorBSpline);
					updateGPUGeometry(tensorGPUGeom, tensor);
					prevTensor = currentTensor;
				}
			}
		}
		break;
		} // end switch sceneMode

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		GLint viewMatrixShader = glGetUniformLocation(shader.getProgram(), "viewMatrix");
		glUniformMatrix4fv(viewMatrixShader, 1, GL_FALSE, &viewMatrix[0][0]);

		GLint projectionMatrixShader = glGetUniformLocation(shader.getProgram(), "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixShader, 1, GL_FALSE, &projectionMatrix[0][0]);

		if (application->getUserInput(GLFW_KEY_TAB))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Generate Surface of revolution if scene 3 is selcted and its in 3D
		if (application->getUserInput(GLFW_KEY_3) && application->getUserInput(GLFW_KEY_0))
		{
			surfaceGPUGeom.bind();
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(surface.verts.size()));
		}

		else if (application->getUserInput(GLFW_KEY_4) && application->getUserInput(GLFW_KEY_0))
		{
			tensorGPUGeom.bind();
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(tensor.verts.size()));
		}

		else
		{
			curveGPUGeom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, GLsizei(curvePts.verts.size()));

			pointsGPUGeom.bind();
			glDrawArrays(GL_POINTS, 0, GLsizei(square.verts.size()));
		}

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
