#version 400
layout (location = 0) in vec3 aPos;

out vec2 fragCoord;

uniform mat4 matrix;

void main()
{   
    gl_Position = matrix * vec4(aPos, 1.0f);
}