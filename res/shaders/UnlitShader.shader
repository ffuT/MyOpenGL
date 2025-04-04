#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;       // Position
layout (location = 1) in vec2 aTexCoords; // Texture Coordinates

out vec2 TexCoords; // Pass texture coordinates to fragment shader

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords; // Interpolated texture coordinates
out vec4 FragColor; // Output color

uniform sampler2D texture1;     // Texture sampler
uniform vec4 u_color;           // Optional solid color

void main() {
    FragColor = vec4(u_color.rgb, 1.0);
}
