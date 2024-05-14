#version 330 core
out vec4 FragColor;

in vec3 FragPos;

const vec3 ambientColor = vec3(0.2, 0.2, 0.2);

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform float ambientReflection;
uniform float diffuseConstant;
uniform float specularConstant;
uniform float shininess;

void main()
{
    vec3 dx = dFdx(FragPos);
    vec3 dy = dFdy(FragPos);
    vec3 normal = normalize(cross(dx, dy));

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalize(normal));

    float diffuse = max(dot(normalize(normal), lightDir), 0.0);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float ambient = ambientReflection;

    vec3 result = (ambient* 0.1 * vec3(0.0,0.5,1.0) + diffuseConstant * diffuse * vec3(0.0,0.5,1.0)+ specularConstant * specular * lightColor);

    FragColor = vec4(result, 1.0);
}
