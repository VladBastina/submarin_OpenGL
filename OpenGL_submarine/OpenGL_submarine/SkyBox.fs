#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform float mixValue;

void main()
{
    vec4 black = vec4(0.0,0.0,0.0,1.0);
    vec4 skyboxcolor =texture(skybox, TexCoords);
    FragColor = mix(black,skyboxcolor,mixValue);
}
