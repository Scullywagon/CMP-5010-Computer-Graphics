#version 330 core
out vec4 FragColor;


in vec3 FragPos; 
in vec3 Normal; 
in vec2 TexCoords; 

struct TexturedMaterial 
{
    sampler2D diffuse;
    sampler2D normal;
  //  sampler2D specular;
   // sampler2D height;
};

struct UntexturedMaterial 
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 sunDir;
uniform vec3 sunAmbient;
uniform vec3 sunDiffuse;
uniform vec3 sunSpecular;

uniform vec3 cameraPos;

uniform bool isTextured = false;
uniform UntexturedMaterial material;

uniform Light lights[24];
uniform int numLights;

vec3 calculateSunlight(vec3 texColor, vec3 norm);
vec3 calculateGeneralLight(Light l, vec3 texColor, vec3 norm);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 texColor = texture(material.diffuse, TexCoords).rgb;


    vec3 result = calculateSunlight(texColor, norm);

    for (int i = 0; i < numLights; i++)
    {
        result += calculateGeneralLight(lights[i], texColor, norm);
    }

    FragColor = vec4(result, 1.0);
}

vec3 calculateSunlight(vec3 texColor, vec3 norm)
{
    vec3 ambient = sunAmbient * texColor;

    // diffuse for sun
    vec3 lightDir = normalize(-sunDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = sunDiffuse * diff * texColor;

    // specular for sun
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = sunSpecular * spec * texColor;

    vec3 result = specular + diffuse + ambient;
    return result;
}

vec3 calculateGeneralLight(Light l, vec3 texColor, vec3 norm)
{
    float distance = length(l.position - FragPos);
    float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * (distance * distance));

    // ambient
    vec3 ambient = l.ambient * texColor * attenuation;

    // diffuse
    vec3 lightDir = normalize(l.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * l.diffuse * texColor * attenuation;

    // specular lighting
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = l.specular * spec * texColor * attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}
