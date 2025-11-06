#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTex;

out vec2 TexCoords;

void main() {
    vec2 pos = aPos * 0.24;           // scale 
    pos += vec2(0.75, 0.75);          // move it to top-right
    TexCoords = aTex;
    gl_Position = vec4(pos, 0.0, 1.0);
}

#shader fragment
#version 330 core
in vec2 TexCoords;
uniform sampler2D u_depthMap;
out vec4 FragColor;

void main() {
    float depth = texture(u_depthMap, TexCoords).r;
    FragColor = vec4(vec3(depth), 1.0); // visualize depth
}