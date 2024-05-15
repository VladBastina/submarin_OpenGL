#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float mixValue;
uniform vec3 viewPos;

in vec3 FragPos;

void main()
{
    float x = abs(FragPos.x);
    float z = abs(FragPos.z);

    float trueMix = mixValue - 0.5;
    if (trueMix < 0.1) {
        trueMix = 0.1;
    }

    vec2 texCoords = vec2(x, z) / 5.0;
    vec4 texture1Color = texture(texture1, texCoords);
    vec4 texture2Color = texture(texture2, texCoords);
    vec4 mixedColor = mix(texture1Color, texture2Color, 0.25);
    vec4 baseColor = mix(vec4(0.0, 0.0, 0.0, 1.0), mixedColor, trueMix);

    float distance = length(viewPos - FragPos);

    float fogFactor = exp(-fogDensity * distance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 finalColor = mix(fogColor, baseColor.rgb, fogFactor);

    FragColor = vec4(finalColor, baseColor.a);
}
