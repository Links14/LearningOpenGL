#version 330 core
out vec4 FragColor;

// Inputs the color from the vertex shader
in vec3 color;
// Inputs the texture coordinates from the vertex shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
};