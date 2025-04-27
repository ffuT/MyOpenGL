#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    FragPos = vec3(u_model * vec4(aPos, 1.0)); // World-space position
    gl_Position = u_proj * u_view * vec4(FragPos, 1.0);

    Normal = mat3(transpose(inverse(u_model))) * aNormal; // Correct normals
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 u_viewPos;
uniform vec4 u_color;
uniform float u_specularStrength;

float ambientStrength = 0.01;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light lights[32];  
uniform int numLights;

void main() {
    vec3 result = vec3(0.0);
    
    for (int i = 0; i < numLights; i++) {
        vec3 ambient = ambientStrength * lights[i].color;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        vec3 viewDir = normalize(u_viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = u_specularStrength * spec * lights[i].color;

        result += (ambient + diffuse + specular) * u_color.rgb;
    }

    FragColor = vec4(result, u_color.a);
}
