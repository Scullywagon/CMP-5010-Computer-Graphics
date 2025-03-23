#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec3 Normal; 
in vec2 TexCoords; 

uniform vec3 lightPos;  
uniform vec3 lightColor;  
uniform vec3 objectColor;  

uniform sampler2D texture1; 

void main()
{
    vec3 texColor = texture(texture1, TexCoords).rgb; 

    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor * texColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal); 
    vec3 lightDir = normalize(lightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = diff * lightColor * texColor; // Apply texture to diffuse lighting

    // Final result
    vec3 finalColor = ambient + diffuse;
    FragColor = vec4(finalColor, 1.0); // Ensure alpha is explicitly set to 1.0
}
