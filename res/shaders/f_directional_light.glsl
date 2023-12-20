#version 330 core

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

uniform struct Light {
    vec3 position;       // Позиция света
    vec3 direction;       // Направление света
    vec3 color;           // Цвет света
    float intensity;      // Интенсивность света
    float constant;
    float linear;
    float quadratic;
    float cutOff;
} light;

uniform vec3 ViewPos;
uniform int lightingMethod;
uniform int lighting_type;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    if(lighting_type == 1) lightDir = normalize(-light.direction);

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(ViewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 specular = vec3(0.0);
    vec3 result = vec3(1.0, 1.0, 1.0);

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));
    attenuation*=light.intensity;

    float theta = dot(lightDir, normalize(-light.direction));

    switch(lightingMethod) {
        case 0: { // Phong
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular = spec * light.color * material.specularColor;
            result = (material.ambientColor + light.intensity * (diff * material.diffuseColor + specular) + material.emissionColor);
            if(lighting_type == 0) result*=attenuation;
            if(lighting_type == 2 && theta < light.cutOff) result = material.ambientColor;
            break;
        }
        case 1: { // Toon
            float toonDiff = floor(diff * 5.0) / 5.0; // 5.0 - количество уровней освещенности
            float specThreshold = 0.2;
            float spec = step(specThreshold, max(dot(viewDir, reflectDir), 0.0)) * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular = spec * light.color * material.specularColor;
            result = (material.ambientColor + toonDiff * material.diffuseColor + specular);
            if(lighting_type == 0) result*=attenuation;
            if(lighting_type == 2 && theta < light.cutOff) result = material.ambientColor;
            break;
        }
        case 2: { // Rim
            vec3 rimColor = vec3(0.8, 0.0, 0.2);
	        float bias = 0.3;
	        float rimPower = 8.0;

            vec3 h = lightDir + viewDir;
	        h = normalize(h / length(h));

            result = material.emissionColor + material.ambientColor;
	        float Ndot = max(dot(norm, lightDir), 0.0);
	        result += material.diffuseColor * Ndot;

	        float RdotVpow = max(pow(dot(norm, h), material.shininess), 0.0);
	        result += material.specularColor * RdotVpow;

	        result += rimColor * pow(1.0 + bias - max(dot(norm, viewDir), 0.0), rimPower);
            if(lighting_type == 0) result*=attenuation;
            if(lighting_type == 2 && theta < light.cutOff) result = material.ambientColor;
            break;
        }
    }

    // Добавление текстуры
    vec4 textureColor = texture(texture1, TexCoord);
    result *= textureColor.rgb;

    FragColor = vec4(result, 1.0);
}