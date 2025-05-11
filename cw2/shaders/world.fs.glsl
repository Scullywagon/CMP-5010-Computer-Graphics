#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 fragPosLightSpace;

// scrap these -- ditching normal maps due to workload and memory usage
// let the other values get properly set though
// two mat could be using double per material
// check for rgba32f -- expensice
/*
struct TexturedMaterial 
{
    sampler2D diffuse;
    sampler2D normal;
    vec3 specular;
    float shininess;
};
*/

struct Material 
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

//uniform bool isTextured = false;
//uniform UntexturedMaterial material;
uniform Material material;

uniform sampler2D depthMap;

uniform Light lights[50];
uniform int numLights;

uniform bool isLight; 
uniform vec3 outputColor;


vec4 calculateSunlight(vec4 texColor, vec3 norm, float shininess, float shadow);
vec4 calculateGeneralLight(Light l, vec4 texColor, vec3 norm, float shininess);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm); 

void main()
{
    vec4 texColor;
    vec3 norm;
    float shininess;
        norm = normalize(Normal);
        texColor = texture(material.diffuse, TexCoords);
        shininess = material.shininess;

    if (texColor.a < 0.5)
    {
        discard;
    }
    
    float shadow = ShadowCalculation(fragPosLightSpace, norm);
    vec4 result = calculateSunlight(texColor, norm, shininess, shadow);

    for (int i = 0; i < numLights; i++)
    {
        result += calculateGeneralLight(lights[i], texColor, norm, shininess);
    }

    if (isLight)
    {
        result += vec4(outputColor, 1.0);
    }

    FragColor = result;
}

vec4 calculateSunlight(vec4 texColor, vec3 norm, float shininess, float shadow)
{
    vec3 ambient = sunAmbient * texColor.rgb;

    vec3 lightDir = normalize(-sunDir);
    float diff = max(dot(norm, lightDir), 0.0);
    if (diff < 0.1 || shadow > 0.5)
        return vec4(ambient, texColor.a); 
    vec3 diffuse = sunDiffuse * diff * texColor.rgb;
    
    vec3 specular = vec3(0.0);
    if (shadow < 0.5)
    {
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = sunSpecular * spec * texColor.rgb;
    }

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));
    return vec4(result, texColor.a); // Retain the alpha value
}

vec4 calculateGeneralLight(Light l, vec4 texColor, vec3 norm, float shininess)
{
    float distance = length(l.position - FragPos);
    if (distance < 0.1 || distance > 50.0)
        return vec4(0.0, 0.0, 0.0, texColor.a); 
    float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * (distance * distance));

    vec3 ambient = l.ambient * texColor.rgb * attenuation;

    vec3 lightDir = normalize(l.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    if (diff < 0.1)
        return vec4(ambient, texColor.a);
    vec3 diffuse = diff * l.diffuse * texColor.rgb * attenuation;

    vec3 specular = vec3(0.0);
    if (attenuation > 0.3)
    {
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = l.specular * spec * texColor.rgb * attenuation;
    }

    vec3 result = ambient + diffuse + specular;
    return vec4(result, texColor.a); 
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 0.0;

    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float bias = max(0.0005 * (1.0 - dot(norm, sunDir)), 0.0005);  
 
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
int sampleRadius = 1; // Can be adjusted based on performance needs
    
    for(int x = -sampleRadius; x <= sampleRadius; x += 1) {
        for(int y = -sampleRadius; y <= sampleRadius; y += 1) {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (projCoords.z - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    
    int samples = (2 * sampleRadius + 1) * (2 * sampleRadius + 1);
    return shadow / float(samples);
}
