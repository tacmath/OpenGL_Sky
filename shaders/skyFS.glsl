#version 400
out vec4 FragColor;

in vec2 fragCoord;

uniform float time;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = (fragCoord + 1) * 0.5f;

    // Time varying pixel color
    vec3 col = 0.5 + 0.5*cos(time + uv.xyx+vec3(0,2,4));
    FragColor = vec4(col, 1.0);
}