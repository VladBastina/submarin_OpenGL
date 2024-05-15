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
uniform float mixValue;

uniform vec3 fogColor;
uniform float fogDensity;

uniform samplerCube skybox;
uniform sampler2D floor;

void main()
{
    vec3 dx = dFdx(FragPos);
    vec3 dy = dFdy(FragPos);
    vec3 normal = normalize(cross(dx, dy));

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diffuse = max(dot(normal, lightDir), 0.0);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float ambient = ambientReflection;

    vec3 result = (ambient * ambientColor + 
                  diffuseConstant * diffuse * vec3(0.0, 0.5, 1.0) + 
                  specularConstant * specular * lightColor);

    vec4 color = vec4(result, 1.0);

    if(viewPos.y > 0.0)
    {
        vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normal);
        vec4 reflexion = vec4(texture(skybox, R).rgb, 0.1);
        vec4 skyboxreflect = mix(reflexion, color, 0.2);
        skyboxreflect = mix(black, skyboxreflect, mixValue);

        float ratio = 1.33;
        vec3 Rr = refract(I, normal, ratio);
        vec4 refraction = vec4(texture(floor, vec2(Rr.xy) / 2.0).rgb, 1.0);

        FragColor = mix(skyboxreflect, refraction, 0.2);
    }
    else
    {
        vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 baseColor = color;

        float ratio = 1.0 / 1.33;
        vec3 Ir = normalize(FragPos - viewPos);
        vec3 Rr = refract(Ir, normal, ratio);
        vec4 refraction = vec4(texture(skybox, Rr).rgb, 1.0);

        float distance = length(viewPos - FragPos);
        float fogFactor = exp(-fogDensity * distance);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        vec3 finalColor = mix(fogColor, mix(refraction.rgb, baseColor.rgb, 0.2), fogFactor);

        FragColor =mix(black,vec4(finalColor, baseColor.a),mixValue);
    }
}
