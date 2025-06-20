#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos; 

uniform mat4 u_lightProjection;
uniform mat4 u_model;

void main()
{
	gl_Position = u_lightProjection * u_model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core	

void main()
{
}