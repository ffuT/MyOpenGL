#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;       // Position
layout (location = 1) in vec2 aTexCoords; // Texture Coordinates
layout (location = 2) in vec3 aNormal;    // Normal

out vec2 TexCoords;   // Pass to fragment shader
out vec3 FragPos;     // World-space position
out vec3 Normal;      // Normal vector

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    // Transform vertex position
    FragPos = vec3(u_model * vec4(aPos, 1.0)); // World-space position
    gl_Position = u_proj * u_view * vec4(FragPos, 1.0);

    // Pass normal and texture coordinates
    Normal = mat3(transpose(inverse(u_model))) * aNormal; // Correct normals
    TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture1;     // Texture sampler
uniform vec3 u_lightPos;        // Light position
uniform vec3 u_lightColor;      // Light color
uniform vec3 u_Color;           // Object color
uniform vec3 u_viewPos;
uniform float u_ambientStrength;
uniform float u_specularStrength;

void main() {

   // Ambient lighting
    float ambientStrength = u_ambientStrength;
    vec3 ambient = ambientStrength * u_lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // Specular lighting (optional)
    float specularStrength = u_specularStrength;
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * u_Color;
    FragColor = vec4(result, 1.0);
}