#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    //vec3 direction; // Nokta ışıklandırma (point lights) kullanacağımızdan direction'a gerek yok.

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

void main()
{
    // 1) ambient light ayarı
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // 2) diffuse color'un hesaplanması:
    // Işık kaynağının normalleştirilmesi gerekli
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // 3) specular reflection hesaplanması:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // emmision map yanlışlıkla dedektif numarası keşfettim. 
    // Çünkü ışığın vurduğu yerde görünüyor sadece. 
    // Allah'ım sana şükürler olsun. Bunu çok güzel fikirlerde kullanabilirim.
    vec3 emission = light.specular * spec * texture(material.emission, TexCoords).rgb;
    // vec3 emission = texture(material.emission, TexCoords).rgb;

    // Noktasal ışıklandırmanın zayıflamasının hesaplanabilmesi için:
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    // phong lighting oluşturulması:
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}