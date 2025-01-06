#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(aPos.x, aPos.z, aPos.y, 1.0);
}