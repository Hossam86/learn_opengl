#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec2 texCoord;

uniform mat4 model; 
uniform mat4 view;
uniform projectionl

void main()
{
    gl_Position =projectionl * view *model *vec4(aPos, 1.0f);
}