#version 460 core

layout (location = 0) in vec2 aPos;

uniform mat4 model;         // position of model
uniform mat4 view;          // position of camera 
uniform mat4 projection;    // projection of scene to camera space

void main()
{
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
};
