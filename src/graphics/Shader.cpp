#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#include "Shader.h"

Shader::Shader(const std::string& filepath) :
    m_FilePath(filepath), m_RendererID(0) {
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);  // Pass geometry shader source
}


Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class Shadertype {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };

    std::string line;
    std::stringstream ss[3]; 
    Shadertype type = Shadertype::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = Shadertype::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = Shadertype::FRAGMENT;
            }
            else if (line.find("geometry") != std::string::npos) { 
                type = Shadertype::GEOMETRY;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str(), ss[2].str() };
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {   // If compilation fails 
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)malloc(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!\n";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    unsigned int gs = 0;

    // If a geometry shader is provided, compile and attach it
    if (!geometryShader.empty()) {
        gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
        glAttachShader(program, gs);
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    if (gs) glDeleteShader(gs);  // Delete the geometry shader if it exists

    return program;
}


void Shader::SetUniform1f(const std::string name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const std::string name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::UnBind() const {
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform4f(const std::string name, float* in) {
    glUniform4f(GetUniformLocation(name), in[0], in[1], in[2], in[3]);
}

void Shader::SetUniform4f(const std::string name, glm::vec4 vec) {
    glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniformMat4f(const std::string name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniform3f(const std::string name, float* in) {
    glUniform3f(GetUniformLocation(name), in[0], in[1], in[2]);
}

void Shader::SetUniform3f(const std::string name, glm::vec3 vec) {
    glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCahce.find(name) != m_UniformLocationCahce.end())
        return m_UniformLocationCahce[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "warning: uniform " << name << " doesnt exist " << std::endl;
    m_UniformLocationCahce[name] = location;
    return location;
}
