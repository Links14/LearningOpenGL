#version 330 core
out vec4 FragColor;

// Inputs the color from the vertex shader
in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
};