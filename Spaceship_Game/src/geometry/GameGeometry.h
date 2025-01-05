#ifndef GAME_GEOMETRY_H
#define GAME_GEOMETRY_H

#include "Geometry.h"
#include "../graphics/Texture.h"
#include <string>

struct GameGeometry
{
    GameGeometry(CPU_Geometry cpg, std::string path, GLint interpolation);

    CPU_Geometry cgeom;
    GPU_Geometry ggeom;
    Texture texture;
};

#endif // GAME_GEOMETRY_H