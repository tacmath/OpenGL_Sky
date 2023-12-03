#version 400
out vec4 FragColor;

in vec3 fragCoord;

uniform float time = 0;
uniform vec3 sunPos;
//vec3 sunPos = vec3(0.0f, 0.0f, 1.0f);

//uniform vec3 sunPos = vec3(0, 0, 1);
//uniform float cumulus = 0.6;
const float Br = 0.0025;
const float Bm = 0.0003;
const float g =  0.9800;
const vec3 nitrogen = vec3(0.650, 0.570, 0.475);
const vec3 Kr = Br / pow(nitrogen, vec3(4.0));
const vec3 Km = Bm / pow(nitrogen, vec3(0.84));

#define COLOR1 vec3(0.0960, 0.571, 0.960)
#define COLOR2 vec3(0.781, 0.863, 0.930)

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

//  const mat3 m = mat3(0.0, 1.60,  1.20, -1.6, 0.72, -0.96, -1.2, -0.96, 1.28);
//  float fbm(vec3 p)
//  {
//    float f = 0.0;
//    f += noise(p) / 2; p = m * p * 1.1;
//    f += noise(p) / 4; p = m * p * 1.2;
//    f += noise(p) / 6; p = m * p * 1.3;
//    f += noise(p) / 12; p = m * p * 1.4;
//    f += noise(p) / 24;
//    return f;
//  }

uniform vec3 SkyColor = vec3(0);
uniform vec3 VoidColor = vec3(0);
uniform vec3 NightColor = vec3(0);
uniform vec3 SunColor = vec3(0);

void main()
{
    vec3 coord = normalize(fragCoord); // try to not normalize to see the difference
    float mu = dot(normalize(coord), sunPos);
 //   float rayleigh = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu);
 //   vec3 mie = (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm);
//
//    vec3 day_extinction = exp(-exp(-((coord.y + sunPos.y * 4.0) * (exp(-coord.y * 16.0) + 0.1) / 80.0) / Br) * (exp(-coord.y * 16.0) + 0.1) * Kr / Br) * exp(-coord.y * exp(-coord.y * 8.0 ) * 4.0) * exp(-coord.y * 2.0) * 4.0;
//    vec3 night_extinction = vec3(1.0 - exp(sunPos.y)) * 0.2;
//    vec3 extinction = mix(day_extinction, night_extinction, -sunPos.y * 0.2 + 0.5);
//    vec3 color = rayleigh * mie * extinction;

        // Cumulus Clouds
//    for (int i = 0; i < 3; i++)
//    {
//      float density = smoothstep(1.0 - cumulus, 1.0, fbm((0.7 + float(i) * 0.01) * coord.xyz / coord.y + time * 0.5));
//      color.rgb = mix(color.rgb, extinction * density * 5.0, min(density, 1.0) * max(coord.y, 0.0));
//    }

    float sunMu = clamp(mu, 0.0f, 1.0f);
    vec3 daySkyColor = mix(VoidColor, SkyColor, smoothstep(-1.0f, 0.5f, coord.y));//-coord.y * 0.5f + 0.5f);
    vec3 extinctionColor = mix(daySkyColor, SunColor, pow(1.0f - sunPos.y * sunPos.y, 2) * (0.4f + 0.8f * sunMu * sunMu)); // sunMu * sunMu + pow(1.0f - sunPos.y * sunPos.y, 2)
    vec3 skyColor = mix(extinctionColor, NightColor, -sunPos.y * 0.5f + 0.5f);
    vec3 rayLight = sunMu * sunMu * 0.5f * extinctionColor; //+ 1.0f;
    float sun = pow(sunMu, 30);
    vec3 color = skyColor + sun;


    color += noise(coord * 1000) * 0.01;

    //color = (coord.y > -0.01 && coord.y < 0.01) ? vec3(0) : color; // horizon

  //  vec3 day_extinction = exp(-exp(-((pos.y + fsun.y * 4.0) * (exp(-pos.y * 16.0) + 0.1) / 80.0) / Br) * (exp(-pos.y * 16.0) + 0.1) * Kr / Br) * exp(-pos.y * exp(-pos.y * 8.0 ) * 4.0) * exp(-pos.y * 2.0) * 4.0;
    
   // vec3 color = mix(mix(COLOR2, COLOR1, smoothstep(-1.0f, 0.2f, coord.y)), vec3(1.0f), smoothstep(0, -1.0f, exp(dot(sunPos, fragCoord))));
 //   color = pow(1.0 - exp(-1.3 * color), vec3(1.3));
    FragColor = vec4(color, 1.0f);
}