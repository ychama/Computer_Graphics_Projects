#include "Application.h"

#include "GLFW/glfw3.h"
#include "../utils/GLDebug.h"

#include "ShaderSceneCallbacks.h"
#include "../geometry/Fractals.h"
#include "../graphics/Window.h"
#include "../utils/Log.h"

Application::Application(int width, int height)
    : window(width, height, "Fractal Generator"),
      shader("shaders/test.vert", "shaders/test.frag")
{

    GLDebug::enable();

    callbacks = std::make_shared<ShaderSceneCallbacks>(shader);
    window.setCallbacks(callbacks);

    // Create geometry
    p = callbacks->getParameters();
    cpuGeom = render_scenes(p);
    gpuGeom.setVerts(cpuGeom.verts);
    gpuGeom.setCols(cpuGeom.cols);
}

void Application::run()
{
    // Render loop
    while (!window.shouldClose())
    {
        glfwPollEvents();

        // Parameter changes
        Parameters newP = callbacks->getParameters();
        if (newP.isDifferent(p))
        {
            p = newP;
            cpuGeom = render_scenes(p);
            gpuGeom.setVerts(cpuGeom.verts);
            gpuGeom.setCols(cpuGeom.cols);
        }

        shader.use();
        gpuGeom.bind();

        glEnable(GL_FRAMEBUFFER_SRGB);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        switch (p.scene)
        {
        case 1:
            glDrawArrays(GL_TRIANGLES, 0, cpuGeom.verts.size());
            break;
        case 2:
            glDrawArrays(GL_LINE_LOOP, 0, cpuGeom.verts.size());
            break;
        case 3:
            glDrawArrays(GL_LINES, 0, cpuGeom.verts.size());
            break;
        case 4:
            glDrawArrays(GL_LINES, 0, cpuGeom.verts.size());
            break;
        }

        glDisable(GL_FRAMEBUFFER_SRGB);
        window.swapBuffers();
    }

    // Cleanup
    glfwTerminate();
}
