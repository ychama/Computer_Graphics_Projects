#ifndef SHADER_SCENE_CALLBACKS_H
#define SHADER_SCENE_CALLBACKS_H

#include "../graphics/Window.h"
#include "../graphics/ShaderProgram.h"
#include "Parameters.h"

class ShaderSceneCallbacks : public CallbackInterface
{
public:
    explicit ShaderSceneCallbacks(ShaderProgram &shader);

    virtual void keyCallback(int key, int scancode, int action, int mods) override;

    Parameters getParameters() const;

private:
    ShaderProgram &m_shader;
    Parameters m_parameters;
};

#endif // SHADER_SCENE_CALLBACKS_H
