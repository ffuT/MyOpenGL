#include "Skybox.h"
#include <stb_image/stb_image.h>
#include <iostream>

static GLuint loadCubemap(std::string path) {
    std::vector<std::string> faces{
    path + "right.jpg",
    path + "left.jpg",
    path + "top.jpg",
    path + "bottom.jpg",
    path + "front.jpg",
    path + "back.jpg"
    };

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

Skybox::Skybox(std::string imagePath, std::string shaderPath) :
	shader(Shader(shaderPath)),
	VBO(108 * sizeof(float), skyboxVertices)
{
	VAO.Bind();
	VAO.AddVertexBuffer(VBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	VAO.Unbind();
	VBO.Unbind();

	shader.Bind();
	shader.UnBind();

    cubemapTexture = loadCubemap(imagePath);
}

Skybox::~Skybox(){
}

void Skybox::Render(glm::mat4 view, glm::mat4 proj){
    glDepthFunc(GL_LEQUAL); 
    shader.Bind();
    shader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(view))); 
    shader.SetUniformMat4f("u_proj", proj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36); 
    VAO.Unbind();

    shader.UnBind();
    glDepthFunc(GL_LESS);  
}
