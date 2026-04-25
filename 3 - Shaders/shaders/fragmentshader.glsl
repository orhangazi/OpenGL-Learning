#version 460 core
out vec4 FragColor;

//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
//uniform vec3 ourColor; // We set this variable in openGL code.

in vec3 ourColor;

void main() 
{
    FragColor = vec4(ourColor, 1.0);
}