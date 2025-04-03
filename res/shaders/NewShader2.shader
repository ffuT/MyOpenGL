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

uniform sampler2D texture1; // Texture sampler
uniform vec3 u_viewPos;
uniform vec4 u_color;
uniform float u_specularStrength;

float ambientStrength = 0.01;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light lights[16];  // Array of lights (assuming a maximum of 10 lights)
uniform int numLights;     // Number of lights (to limit the loop)

void main() {
    vec3 result = vec3(0.0);
    
    // Loop through each light source
    for (int i = 0; i < numLights; i++) {
        // Ambient lighting
        vec3 ambient = ambientStrength * lights[i].color;

        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        // Specular lighting
        vec3 viewDir = normalize(u_viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = u_specularStrength * spec * lights[i].color;

        // Combine the results
        result += (ambient + diffuse + specular) * u_color.rgb;
    }

    // Use the result of lighting calculation
    FragColor = vec4(result, u_color.a);
}
