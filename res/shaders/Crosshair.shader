#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * u_view * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec3 u_lineColor;

void main() {
    FragColor = vec4(u_lineColor, 1.0);
}