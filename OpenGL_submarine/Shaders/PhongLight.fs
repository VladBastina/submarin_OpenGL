#version 330 core

in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

const vec3 ambientColor = vec3(0.2, 0.2, 0.2);

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform float ambientReflection;
uniform float diffuseConstant;
uniform float specularConstant;
uniform float shininess;
uniform float mixValue;

uniform vec3 fogColor;
uniform float fogDensity;

uniform sampler2D texture_diffuse;

void main()
{
    vec3 dx = dFdx(FragPos);
    vec3 dy = dFdy(FragPos);
    vec3 normal = normalize(cross(dx, dy));

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = ambientReflection * ambientColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseConstant * diff * lightColor;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularConstant * spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    vec4 textureColor = texture(texture_diffuse, TexCoords);

    vec4 color = mix(vec4(result, 1.0), textureColor, 0.8);

    vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 baseColor = color;

    float distance = length(viewPos - FragPos);
    float fogFactor = exp(-fogDensity * distance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 finalColor = mix(fogColor,baseColor.rgb, fogFactor);

    FragColor =mix(black,vec4(finalColor, baseColor.a),mixValue);
}
