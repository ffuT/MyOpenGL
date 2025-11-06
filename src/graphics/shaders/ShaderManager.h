#pragma once
#include "Shader.h"

enum ShaderProgram {
	BasicShader,        // buffer rendering
    NewShader,          // basic lighting
    UnlitShader,        // full bright
    WireframeShader,    // triangle lines
    SkyboxShader,       // skybox
    CrosshairShader,    // crosshair
	ShadowShader,       // shadow mapping
};

class ShaderManger {
public:
    ShaderManger();
    ~ShaderManger();

    Shader* GetShader(const ShaderProgram& ShaderName);

private:
    Shader m_BasicShader = Shader("res/shaders/BasicShader.shader");
    Shader m_NewShader = Shader("res/shaders/NewShader2.shader");
    Shader m_UnlitShader = Shader("res/shaders/UnlitShader.shader");
    Shader m_WireframeShader = Shader("res/shaders/Wireframeshader.shader");
    Shader m_SkyboxShader = Shader("res/shaders/Skybox.shader");
    Shader m_CrosshairShader = Shader("res/shaders/Crosshair.shader");
    Shader m_ShadowShader = Shader("res/shaders/Shadow.shader");
};
