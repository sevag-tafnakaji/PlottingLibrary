#version 460 core

layout (location = 0) in vec2 aPos;

uniform float offset_x;
uniform float offset_y;
uniform float scale;

void main()
{
   gl_Position = vec4((aPos.x + offset_x) * scale, (aPos.y + offset_y) * scale, 0.0, 1.0);
};
