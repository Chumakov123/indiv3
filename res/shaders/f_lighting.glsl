#version 330 core
#define MAX_LIGHTS 10

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

uniform struct Material {
    vec3 diffuseColor;    // Диффузный цвет материала
    vec3 specularColor;   // Цвет бликов материала
    vec3 emissionColor;   // Цвет эмиссии материала
    vec3 ambientColor;    // Цвет амбиентной составляющей материала
    float shininess;      // Степень блеска материала
} material;

struct Light {
    int type; // 0 - point, 1 - directional, 2 - spot
    vec3 position;       // Позиция света
    vec3 direction;       // Направление света
    vec3 color;           // Цвет света
    float intensity;      // Интенсивность света
    float constant;
    float linear;
    float quadratic;
    float cutOff;
};

uniform Light lights[MAX_LIGHTS];

uniform vec3 ViewPos;
uniform int numLights;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 result = material.ambientColor;

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        if (lights[i].type == 1) lightDir = normalize(-lights[i].direction);
        if (lights[i].type == 2) {
            float theta = dot(lightDir, normalize(-lights[i].direction));
            if (theta < lights[i].cutOff) continue;
        }

        vec3 reflectDir = reflect(-lightDir, norm);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 specular = vec3(0.0);

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
        attenuation *= lights[i].intensity;

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = spec * lights[i].color * material.specularColor;
        result += lights[i].intensity * (diff * material.diffuseColor + specular);
        if (lights[i].type == 0) result *= attenuation;
    }

    result += material.emissionColor;

    // Добавление текстуры
    vec4 textureColor = texture(texture1, TexCoord);
    result *= textureColor.rgb;

    FragColor = vec4(result, 1.0);
}