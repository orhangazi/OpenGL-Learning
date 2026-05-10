#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 norm = normalize(Normal);

    // ambient
    vec3 ambient = 0.2 * texture(texture_diffuse1, TexCoords).rgb;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(texture_diffuse1, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * texture(texture_specular1, TexCoords).rgb;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
