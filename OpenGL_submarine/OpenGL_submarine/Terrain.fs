#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 FragPos;

void main()
{
    float x;
    float z;
    if(FragPos.x<0)
    {
        x=-FragPos.x;
    }
    else
    {
        x=FragPos.x;
    }
    if(FragPos.z<0)
    {
        z=-FragPos.z;
    }
    else
    {
        z=FragPos.z;
    }
    vec2 texCoords = vec2(x,z) /100;
    vec4 texture1Color = texture(texture1,texCoords);
    vec4 texture2Color = texture(texture2,texCoords);
    vec4 mixedColor = mix(texture1Color, texture2Color, 0.5);
    FragColor = mixedColor;
}