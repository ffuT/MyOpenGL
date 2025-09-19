#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    TexCoords = aPos;    
    vec4 pos = u_proj * mat4(mat3(u_view)) * vec4(aPos, 0.0);
    gl_Position = pos.xyww; 
}

#shader fragment
#version 330 core
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 u_sunDir;   // normalized sun direction
uniform float u_blend;   // 0=space, 1=scattering

const vec3 SCATTER_RGB = vec3(0.3, 0.6, 1.0); 

void main() {    
    vec3 viewDir = normalize(TexCoords);

    // Rayleigh scattering
    float cosTheta = dot(viewDir, u_sunDir);
    float phaseR = 0.75 * (1.0 + cosTheta * cosTheta);
    
    // horizon tint
    float horizon = clamp(1.0 - viewDir.y, 0.0, 0.85);
    float sunHeight = u_sunDir.y; // 1.0 = overhead, 0 = horizon
    float horizonFactor = clamp(1.0 - abs(sunHeight)*2.0, 0.0, 1.0);
    //vec3 horizonTint = mix(SCATTER_RGB, vec3(1.0, 0.2, 0.3), horizonFactor);
    vec3 horizonTint = mix(SCATTER_RGB, vec3(0.8, 0.1, 0.3),  horizonFactor);

    // sun glow
    float sunAngle = dot(viewDir, u_sunDir); 
    float sunGlow = smoothstep(0.995, 1.0, sunAngle);

    float heightFactor = clamp(viewDir.y * 0.5 + 0.5, 0.0, 1.0);
  
    vec3 scatterColor = phaseR * horizonTint * heightFactor;
    scatterColor += sunGlow;

    // add some noise to reduce banding
    scatterColor += (fract(sin(dot(viewDir.xy, vec2(12.9898,78.233))) * 43758.5453) - 0.5) * 0.025;
    scatterColor = mix(scatterColor, scatterColor * heightFactor, 0.05);
    
    vec3 spaceColor = texture(skybox, viewDir).rgb;
    vec3 finalColor = mix(spaceColor, scatterColor, clamp(u_blend, 0.0, 1.0));

    // gamma correction and boost
    float lum = dot(finalColor, vec3(0.2126, 0.7152, 0.0722));
    vec3 mapped = finalColor * (lum / (lum + 1.0)); 
    finalColor = pow(mapped, vec3(1.0/2.2));
    // saturation boost
    float avg = (finalColor.r + finalColor.g + finalColor.b)/3.0;
    finalColor = mix(vec3(avg), finalColor, 2.0);

    FragColor = vec4(finalColor, 1.0);
}
