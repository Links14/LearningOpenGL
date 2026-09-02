#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Outputs the color for the fragment shader
out vec3 color;

uniform float scale;

void main()
{
	gl_Position = vec4(aPos.x + aPos.x * scale,
		aPos.y + aPos.y * scale,
		aPos.z + aPos.z * scale, 1.0);
	// Assigns the colors from the VertexData to the color
	color = aColor;
};