#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main(){
	gl_Position =  u_proj * u_view * u_model * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 Color;

void main(){
	Color = vec4(1.0, 0.0, 0.0, 1.0);
};