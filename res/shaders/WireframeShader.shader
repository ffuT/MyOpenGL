#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

#shader geometry
#version 330 core

layout(triangles) in;   // Input: triangle primitive
layout(line_strip, max_vertices = 6) out; // Output: lines (edges)

in vec2 TexCoords[]; 
out vec2 fragTexCoords;

float u_wireframeWidth = 0.5;

void main() {
    // Loop over the triangle's edges and output lines
    for (int i = 0; i < 3; ++i) {
        int next = (i + 1) % 3; 
        gl_Position = gl_in[i].gl_Position; 
        fragTexCoords = TexCoords[i];
        EmitVertex();

        gl_Position = gl_in[next].gl_Position;
        fragTexCoords = TexCoords[next];
        EmitVertex();

        EndPrimitive();
    }
}

#shader fragment
#version 330 core

in vec2 fragTexCoords;
out vec4 FragColor; 

uniform vec4 u_color;

void main() {
    FragColor = u_color;
}