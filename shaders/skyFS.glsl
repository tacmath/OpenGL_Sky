#version 400
out vec4 FragColor;

in vec3 fragCoord;

uniform float time = 0;
uniform vec3 sunPos;
//vec3 sunPos = vec3(0.0f, 0.0f, 1.0f);

//uniform vec3 sunPos = vec3(0, 0, 1);

  float hash(float n)
  {
    return fract(sin(n) * 43758.5453123);
  }

  float noise(vec3 x)
  {
    vec3 f = fract(x);
    float n = dot(floor(x), vec3(1.0, 157.0, 113.0));
    return mix(mix(mix(hash(n +   0.0), hash(n +   1.0), f.x),
                   mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
               mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
                   mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
  }

uniform vec3 SkyColor = vec3(0);
uniform vec3 VoidColor = vec3(0);
uniform vec3 NightColor = vec3(0);
uniform vec3 SunColor = vec3(0);

void main()
{
    vec3 coord = normalize(fragCoord); // try to not normalize to see the difference
    float mu = dot(normalize(coord), sunPos);

    float sunMu = clamp(mu, 0.0f, 1.0f);
    vec3 daySkyColor = mix(VoidColor, SkyColor, smoothstep(-1.0f, 0.5f, coord.y));//-coord.y * 0.5f + 0.5f);
    vec3 extinctionColor = mix(daySkyColor, SunColor, pow(1.0f - sunPos.y * sunPos.y, 2) * (0.4f + 0.8f * sunMu * sunMu)); // sunMu * sunMu + pow(1.0f - sunPos.y * sunPos.y, 2)
    vec3 skyColor = mix(extinctionColor, NightColor, -sunPos.y * 0.5f + 0.5f);
    vec3 rayLight = sunMu * sunMu * 0.5f * extinctionColor; //+ 1.0f;
    float sun = pow(sunMu, 30);
    vec3 color = skyColor + sun;


    color += noise(coord * 1000) * 0.01;

   // color = (coord.y > -0.001 && coord.y < 0.001) ? vec3(0) : color; // horizon

 //   color = pow(1.0 - exp(-1.3 * color), vec3(1.3));
    FragColor = vec4(color,1.0f);
}