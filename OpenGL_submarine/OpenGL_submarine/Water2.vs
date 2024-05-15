#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

struct GerstnerWave
{
    float amplitude;
    float wavelength;
    float speed;
    float direction;
    float phase;
};

const int NR_WAVES = 10;

uniform GerstnerWave waves[NR_WAVES];

out vec3 FragPos;

void main()
{
    vec3 displacedPosition = aPos;
    for(int i=0;i<NR_WAVES;i++)
    {
        float k = 2.0 * 3.14159 / waves[i].wavelength;
        float omega = 2.0 * 3.14159 / waves[i].speed;
        float displacementX = waves[i].amplitude * cos(k * (displacedPosition.x * cos(waves[i].direction) + displacedPosition.z * sin(waves[i].direction)) - omega * time + waves[i].phase);
        float displacementZ = waves[i].amplitude * cos(k * (displacedPosition.z * cos(waves[i].direction) - displacedPosition.x * sin(waves[i].direction)) - omega * time + waves[i].phase);
        float displacementY = waves[i].amplitude * sin(k * (displacedPosition.x * cos(waves[i].direction) + displacedPosition.z * sin(waves[i].direction)) - omega * time + waves[i].phase);   
        displacedPosition += vec3(displacementX / 10, displacementY  / 10, displacementZ / 10);
    }

    FragPos = vec3 (model * vec4(displacedPosition,1.0));

    gl_Position = projection * view * model * vec4(displacedPosition, 1.0);
}
