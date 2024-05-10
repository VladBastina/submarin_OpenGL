#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 color;

void main()
{
    vec4 texColor = texture(skybox, TexCoords);
    FragColor = mix(texColor, vec4(color, 1.0), 0.5);
}
