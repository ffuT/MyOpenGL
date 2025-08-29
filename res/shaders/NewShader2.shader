#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_lightSpace;

void main() {
    FragPos = vec3(u_model * vec4(aPos, 1.0)); // World-space position
    gl_Position = u_proj * u_view * vec4(FragPos, 1.0);
    FragPosLightSpace = u_lightSpace * vec4(FragPos, 1.0);

    Normal = mat3(transpose(inverse(u_model))) * aNormal; // Correct normals
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D shadowMap;
uniform vec3 u_viewPos;
uniform vec4 u_color;
uniform float u_specularStrength;

float ambientStrength = 0.025;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    int type; // 0: point, 1: directional 
};

uniform Light lights[32];  
uniform int numLights;

float CalcShadow(vec4 fragPosLightSpace, vec3 lDir){
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calc bias (based on depth map resolution and slope)
    float bias = max(0.003 * (1.0 - dot(Normal, lDir)), 0.0003); 
    float shadow = 0.0;
    // PCF
    vec2 texelSize = 1.1 / textureSize(shadowMap, 0);
    for(int x = -2; x <= 2; ++x){
        for(int y = -2; y <= 2; ++y){
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    return shadow /= 25.0;
}

void main() {
    vec3 result = vec3(0.0);
    
    for (int i = 0; i < numLights; i++) {
        vec3 ambient = ambientStrength * lights[i].color;

        vec3 norm = normalize(Normal);
        vec3 lightDir = (lights[i].type == 0)
            ? normalize(lights[i].position - FragPos) // For point lights
            : normalize(-lights[i].position); // For directional lights

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        vec3 viewDir = normalize(u_viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = u_specularStrength * spec * lights[i].color;

        
        float shadow = 0.0;
        if(lights[i].type == 1)
            shadow = CalcShadow(FragPosLightSpace, lightDir);

        result += (ambient + (1.0 - shadow) * (diffuse + specular)) * u_color.rgb;
    }

    FragColor = vec4(result, u_color.a);
}
