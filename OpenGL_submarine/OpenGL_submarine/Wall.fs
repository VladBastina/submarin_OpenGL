#version 330 core
out vec4 FragColor;

uniform vec3 fogColor;
uniform float fogDensity;
uniform vec3 viewPos;

in vec3 FragPos;

void main()
{
    vec4 baseColor = vec4(0.0,0.12,0.25,1.0);

    float distance = length(viewPos - FragPos);
    float fogFactor = exp(-fogDensity * distance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 finalColor = mix(fogColor, baseColor.rgb, fogFactor);

    FragColor = vec4(finalColor, baseColor.a);
}