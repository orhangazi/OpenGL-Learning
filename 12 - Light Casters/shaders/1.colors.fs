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

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    // 1) ambient light ayarı
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 2) diffuse color'un hesaplanması:
    // Işık kaynağının normalleştirilmesi gerekli
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 3) specular reflection hesaplanması:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // emmision map yanlışlıkla dedektif numarası keşfettim. 
    // Çünkü ışığın vurduğu yerde görünüyor sadece. 
    // Allah'ım sana şükürler olsun. Bunu çok güzel fikirlerde kullanabilirim.
    //vec3 emission = light.specular * spec * vec3(texture(material.emission, TexCoords));
    vec3 emission = vec3(texture(material.emission, TexCoords));

    // phong lighting oluşturulması:
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}