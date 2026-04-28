#version 460 core
out vec4 FragColor;

//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
//uniform vec3 ourColor; // We set this variable in openGL code.
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 TexCoord;

void main() 
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}