#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec3 Normal; 
in vec2 TexCoords; 

float ambientStrength = 0.3;
float specularStrength = 0.3;

struct Material  // you need to figure out how to set this now
{
    sampler2D diffuse; // the texture
    vec3 specular; // the specular color
    float shininess; // how reflective the material is
};

struct Light 
{
    vec3 position;
    vec3 color;
};

uniform vec3 cameraPos;

uniform Material material;
uniform Light light;

void main()
{
    vec3 texColor;
    texColor = texture(material.diffuse, TexCoords).rgb;

    // Ambient lighting
    vec3 ambient = ambientStrength * light.color;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = diff * light.color;

    // specular lighting
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;

    // Final result
    vec3 finalColor = (ambient + diffuse + specular) * texColor;
    FragColor = vec4(finalColor, 1.0); // Ensure alpha is explicitly set to 1.0
}
