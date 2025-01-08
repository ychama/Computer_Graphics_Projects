#include "ShaderSceneCallbacks.h"
#include <GLFW/glfw3.h> // For GLFW_KEY_* constants
#include <iostream>     // If you need std::cout or similar
#include "Parameters.h"
ShaderSceneCallbacks::ShaderSceneCallbacks(ShaderProgram &shader)
    : m_shader(shader)
{
}

void ShaderSceneCallbacks::keyCallback(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        // Recompile shader when 'R' is pressed
        if (key == GLFW_KEY_R)
        {
            m_shader.recompile();
        }
        // Switch scenes
        else if (key == GLFW_KEY_1)
        {
            m_parameters.scene = 1;
        }
        else if (key == GLFW_KEY_2)
        {
            m_parameters.scene = 2;
        }
        else if (key == GLFW_KEY_3)
        {
            m_parameters.scene = 3;
        }
        else if (key == GLFW_KEY_4)
        {
            m_parameters.scene = 4;
        }
        // Change iteration with left/right arrow keys
        else if (key == GLFW_KEY_LEFT)
        {
            if (m_parameters.iteration - m_parameters.iterationStep >= 0)
            {
                m_parameters.iteration -= m_parameters.iterationStep;
            }
        }
        else if (key == GLFW_KEY_RIGHT)
        {
            // Example boundary check, set this to whatever limit you want
            if (m_parameters.iteration + m_parameters.iterationStep < 11)
            {
                m_parameters.iteration += m_parameters.iterationStep;
            }
        }
    }
}

Parameters ShaderSceneCallbacks::getParameters() const
{
    return m_parameters;
}
