#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse;

void main()
{
// simple color blending
//FragColor = vec4(lightColor * objectColor, 1.0);
color=vec4(texture(texture_diffuse, TexCoords));
// ambient
 //   //float ambientStrength = 0.1;
 //   vec3 ambient = 0.5f * lightColor;  	
 //   // diffuse
//float diffuseStrength = 0.5;
 //   vec3 norm = normalize(Normal);
 //   vec3 lightDir = normalize(lightPos - FragPos);
 //   float diff = max(dot(norm, lightDir), 0.0);
 //   vec3 diffuse = diffuseStrength * diff * lightColor;
    
 //   // specular
 //   float specularStrength = 0.5;
 //   vec3 viewDir = normalize(viewPos - FragPos);
 //   vec3 reflectDir = reflect(-lightDir, norm);  
 //   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
 //   vec3 specular = specularStrength * spec * lightColor;  

        
 //   vec3 result = (ambient + diffuse + specular) * objectColor;
 //   FragColor = vec4(result, 1.0);
} 