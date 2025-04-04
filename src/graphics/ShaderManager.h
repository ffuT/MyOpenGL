#pragma once
#include "Shader.h"

enum ShaderProgram {
    NewShader,
    UnlitShader,
    WireframeShader,
    SkyboxShader,
    CrosshairShader
};

class ShaderManger {
public:
    ShaderManger();
    ~ShaderManger();

    Shader* GetShader(const ShaderProgram& ShaderName);

private:
    Shader m_NewShader = Shader("res/shaders/NewShader2.shader");
    Shader m_UnlitShader = Shader("res/shaders/UnlitShader.shader");
    Shader m_WireframeShader = Shader("res/shaders/Wireframeshader.shader");
    Shader m_SkyboxShader = Shader("res/shaders/Skybox.shader");
    Shader m_CrosshairShader = Shader("res/shaders/Crosshair.shader");
};
