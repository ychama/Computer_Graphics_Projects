#include "GameGeometry.h"

GameGeometry::GameGeometry(CPU_Geometry cpg, std::string path, GLint interpolation)
    : cgeom(cpg),
      ggeom(),
      texture(path, interpolation) {}