#pragma once

#include "Geometry.h"  // For CPU_Geometry
#include <glm/glm.hpp> // For glm::vec3, etc.
#include "../core/Parameters.h"

// Declaring fractal functions
void triangle_mass_center(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c,
                          int num_iterations, CPU_Geometry &cpuGeom);

void von_koch_snowflake(glm::vec3 point_a, glm::vec3 point_b,
                        int num_iterations, CPU_Geometry &cpuGeom);

glm::vec3 get_color(int iteration);

void dragon_curve(glm::vec3 point_a, glm::vec3 point_b, int num_iterations,
                  CPU_Geometry &cpuGeom, glm::vec3 color, bool color_is_set);

void s_triangle(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c,
                int num_iterations, CPU_Geometry &cpuGeom, glm::vec3 color);

CPU_Geometry render_scenes(Parameters p);
