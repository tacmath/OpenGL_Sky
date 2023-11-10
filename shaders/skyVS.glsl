#version 400
layout (location = 0) in vec2 aPos;

out vec2 fragCoord;

void main()
{   
    gl_Position = vec4(aPos, 1.0f, 1.0f);
    fragCoord = aPos;
}