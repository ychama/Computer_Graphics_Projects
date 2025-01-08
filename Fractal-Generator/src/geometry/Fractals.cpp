#include "Geometry.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include "../core/Parameters.h"

void triangle_mass_center(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c,
                          int num_iterations, CPU_Geometry &cpuGeom)
{
    glm::vec3 point_d;

    if (num_iterations > 0)
    {
        point_d = (point_a + point_b + point_c) / 3.0f;

        triangle_mass_center(point_a, point_b, point_d, num_iterations - 1, cpuGeom);
        triangle_mass_center(point_a, point_c, point_d, num_iterations - 1, cpuGeom);
        triangle_mass_center(point_b, point_c, point_d, num_iterations - 1, cpuGeom);
    }

    else
    {
        cpuGeom.verts.push_back(point_a);
        cpuGeom.verts.push_back(point_b);
        cpuGeom.verts.push_back(point_c);

        cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));
        cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));
        cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 1.f));
    }
}

void von_koch_snowflake(glm::vec3 point_a, glm::vec3 point_b,
                        int num_iterations, CPU_Geometry &cpuGeom)
{
    if (num_iterations > 0)
    {
        glm::vec3 point_c = (2 / 3.f) * point_a + (1 / 3.f) * point_b;
        glm::vec3 point_d = (2 / 3.f) * point_b + (1 / 3.f) * point_a;

        glm::mat3 rotation_matrix(
            glm::vec3(cos(glm::radians(60.f)), sin(glm::radians(60.f)), 0),
            glm::vec3(-sin(glm::radians(60.f)), cos(glm::radians(60.f)), 0),
            glm::vec3(0, 0, 1));

        glm::vec3 vec_cd = point_d - point_c;
        glm::vec3 point_e = point_a + vec_cd + vec_cd * rotation_matrix;

        von_koch_snowflake(point_a, point_c, num_iterations - 1, cpuGeom);
        von_koch_snowflake(point_c, point_e, num_iterations - 1, cpuGeom);
        von_koch_snowflake(point_e, point_d, num_iterations - 1, cpuGeom);
        von_koch_snowflake(point_d, point_b, num_iterations - 1, cpuGeom);
    }

    else
    {
        cpuGeom.verts.push_back(point_a);
        cpuGeom.verts.push_back(point_b);

        cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));
        cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));
        cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));
    }
}

glm::vec3 get_color(int iteration)
{

    glm::vec3 color_to_use;

    switch (iteration)
    {
    case 0:
        color_to_use = glm::vec3(1.f, 0.f, 0.f);
        break;
    case 1:
        color_to_use = glm::vec3(0.f, 1.f, 0.f);
        break;
    case 2:
        color_to_use = glm::vec3(0.f, 0.f, 1.f);
        break;
    }

    return color_to_use;
}

void dragon_curve(glm::vec3 point_a, glm::vec3 point_b, int num_iterations, CPU_Geometry &cpuGeom, glm::vec3 color, bool color_is_set)
{

    if (num_iterations > 0)
    {

        glm::vec3 point_c = (1 / 2.f) * point_a + (1 / 2.f) * point_b;

        glm::mat3 rotation_matrix(
            glm::vec3(cos(glm::radians(90.f)), sin(glm::radians(90.f)), 0),
            glm::vec3(-sin(glm::radians(90.f)), cos(glm::radians(90.f)), 0),
            glm::vec3(0, 0, 1));

        glm::vec3 vec_ac = point_c - point_a;
        glm::vec3 point_d = point_a + vec_ac + vec_ac * rotation_matrix;

        if (color_is_set)
        {
            dragon_curve(point_a, point_d, num_iterations - 1, cpuGeom, color, true);
            dragon_curve(point_b, point_d, num_iterations - 1, cpuGeom, color, true);
        }
        else
        {
            dragon_curve(point_a, point_d, num_iterations - 1, cpuGeom, get_color((num_iterations - 1) % 3), false);
            dragon_curve(point_b, point_d, num_iterations - 1, cpuGeom, color, true);
        }
    }
    else
    {
        cpuGeom.verts.push_back(point_a);
        cpuGeom.verts.push_back(point_b);

        cpuGeom.cols.push_back(color);
        cpuGeom.cols.push_back(color);
    }
}

void s_triangle(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c, int num_iterations, CPU_Geometry &cpuGeom, glm::vec3 color)
{
    glm::vec3 point_d, point_e, point_f;

    if (num_iterations > 0)
    {
        point_d = 0.5f * point_a + 0.5f * point_b;
        point_e = 0.5f * point_b + 0.5f * point_c;
        point_f = 0.5f * point_c + 0.5f * point_a;

        s_triangle(point_d, point_b, point_e, num_iterations - 1, cpuGeom, glm::vec3(1.0f, 0.f, 0.f));
        s_triangle(point_a, point_d, point_f, num_iterations - 1, cpuGeom, glm::vec3(0.0f, 1.0f, 0.f));
        s_triangle(point_f, point_e, point_c, num_iterations - 1, cpuGeom, glm::vec3(0.f, 0.f, 1.0f));
    }

    else
    {
        cpuGeom.verts.push_back(point_a);
        cpuGeom.verts.push_back(point_b);
        cpuGeom.verts.push_back(point_c);

        cpuGeom.cols.push_back(color);
        cpuGeom.cols.push_back(color);
        cpuGeom.cols.push_back(color);
    }
}

CPU_Geometry render_scenes(Parameters p)
{
    CPU_Geometry cpuGeom;
    glm::vec3 point_a, point_b, point_c;

    switch (p.scene)
    {
    case 1:
        s_triangle(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.5f, -0.5f, 0.f),
                   glm::vec3(0.f, 0.5f, 0.f), p.iteration, cpuGeom,
                   glm::vec3(0.1f, 0.f, 0.f)); // Use GL_TRIANGLE
        break;
    case 2:
        triangle_mass_center(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.5f, -0.5f, 0.f),
                             glm::vec3(0.f, 0.5f, 0.f),
                             p.iteration, cpuGeom); // Use GL_LINE_LOOP
        break;
    case 3:
        point_a = glm::vec3(-0.5f, -0.5f, 0.f);
        point_b = glm::vec3(0.5f, -0.5f, 0.f);
        point_c = glm::vec3(0.f, 0.5f, 0.f);

        von_koch_snowflake(point_a, point_b, p.iteration, cpuGeom);
        von_koch_snowflake(point_b, point_c, p.iteration, cpuGeom);
        von_koch_snowflake(point_c, point_a, p.iteration, cpuGeom);
        break;
    case 4:
        point_a = glm::vec3(-0.5f, 0.f, 0.f);
        point_b = glm::vec3(0.5f, 0.f, 0.5f);

        dragon_curve(point_a, point_b, p.iteration, cpuGeom, get_color(p.iteration % 3), false);
        break;
    }

    return cpuGeom;
}