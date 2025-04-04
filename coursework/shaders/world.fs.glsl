#version 330 core
out vec4 FragColor;


in vec3 FragPos; 
in vec3 Normal; 
in vec2 TexCoords; 
in vec4 fragPosLightSpace;

struct TexturedMaterial 
{
    sampler2D diffuse;
    sampler2D normal;
    vec3 specular;
    float shininess;
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
uniform TexturedMaterial material2;

uniform sampler2D depthMap;

uniform Light lights[24];
uniform int numLights;

vec3 calculateSunlight(vec3 texColor, vec3 norm, float shininess, float shadow);
vec3 calculateGeneralLight(Light l, vec3 texColor, vec3 norm, float shininess);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
    vec3 texColor;
    vec3 norm;
    float shininess;
    if (isTextured)
    {
        //norm = normalize(texture(material2.normal, TexCoords).rgb * 2.0 - 1.0);
        norm = normalize(Normal);
        texColor = texture(material2.diffuse, TexCoords).rgb;
        shininess = material2.shininess;
    }
    else
    {
        norm = normalize(Normal);
        texColor = texture(material.diffuse, TexCoords).rgb;
        shininess = material.shininess;
    }

    float shadow = ShadowCalculation(fragPosLightSpace);
    vec3 result = calculateSunlight(texColor, norm, shininess, shadow);

    for (int i = 0; i < numLights; i++)
    {
        result += calculateGeneralLight(lights[i], texColor, norm, shininess);
    }

    FragColor = vec4(result, 1.0);
}

vec3 calculateSunlight(vec3 texColor, vec3 norm, float shininess, float shadow)
{
    vec3 ambient = sunAmbient * texColor;

    // diffuse for sun
    vec3 lightDir = normalize(-sunDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = sunDiffuse * diff * texColor;

    // specular for sun
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = sunSpecular * spec * texColor;

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));
    return result;
}

vec3 calculateGeneralLight(Light l, vec3 texColor, vec3 norm, float shininess)
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = l.specular * spec * texColor * attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1]
    projCoords = projCoords * 0.5 + 0.5;

    // Depth from shadow map
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    // Shadow bias to prevent acne
    float bias = 0.5;

    // Is fragment in shadow?
    float shadow = currentDepth + bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}
