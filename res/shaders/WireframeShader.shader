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

#shader geometry
#version 330 core

layout(triangles) in;         // Input: triangle primitive
layout(line_strip, max_vertices = 6) out; // Output: lines (edges)

in vec2 TexCoords[]; // Texture coordinates from vertex shader
out vec2 fragTexCoords; // Pass texture coordinates to fragment shader

uniform float u_wireframeWidth;

void main() {
    // Loop over the triangle's edges and output lines
    for (int i = 0; i < 3; ++i) {
        int next = (i + 1) % 3;  // Next vertex, wrapping around the triangle
        gl_Position = gl_in[i].gl_Position; // Set the current vertex position
        fragTexCoords = TexCoords[i];
        EmitVertex();

        gl_Position = gl_in[next].gl_Position; // Set the next vertex position
        fragTexCoords = TexCoords[next];
        EmitVertex();

        EndPrimitive();
    }
}

#shader fragment
#version 330 core

in vec2 fragTexCoords; // Texture coordinates from geometry shader
out vec4 FragColor;    // Output color

uniform vec4 u_color; // Solid color for the wireframe

void main() {
    FragColor = u_color; // Set the wireframe color
}