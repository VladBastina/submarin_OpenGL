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

uniform samplerCube skybox;

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

    vec4 color = vec4(result, 0.1);

    if(viewPos.y>0)
    {
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normalize(normal));
        vec4 reflexion = vec4(texture(skybox, R).rgb, 0.1);
        FragColor = mix(reflexion , color , 0.2);
    }
    else
    {
        float ratio = 1.0 / 1.33;
        vec3 Ir = normalize(FragPos - viewPos);
        vec3 Rr = refract(Ir, normalize(normal), ratio);
        vec4 refraction = vec4(texture(skybox, Rr).rgb, 0.1);
        FragColor = mix(refraction , color , 0.2);
    }
}
