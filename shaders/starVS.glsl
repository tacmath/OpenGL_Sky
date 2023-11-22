#version 400
layout (location = 0) in vec3 aPos;

out float alpha;

uniform mat4 matrix;
uniform vec3 sunPos;

void main()
{   
    gl_Position = matrix * vec4(aPos, 1.0f);
    alpha = smoothstep(-0.2f, 1.0f, dot(sunPos, vec3(0, -1, 0)));
}