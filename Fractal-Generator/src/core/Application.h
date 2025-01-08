#pragma once

#include "../graphics/Window.h"
#include "../graphics/ShaderProgram.h"
#include "../geometry/Fractals.h" // for render_scenes()
#include <memory>
#include "ShaderSceneCallbacks.h"
#include "Parameters.h"

class Application
{
public:
    Application(int width, int height);
    void run();

private:
    Window window;
    ShaderProgram shader;
    std::shared_ptr<ShaderSceneCallbacks> callbacks;

    Parameters p;
    CPU_Geometry cpuGeom;
    GPU_Geometry gpuGeom;
};
