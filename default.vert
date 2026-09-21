#version 330 core
// Position/Coordinates
layout (location = 0) in vec3 aPos;
// Color
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Outputs the color for the fragment shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Controls the scale of the vertices
uniform float scale;

// Model, View, Projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	// Outputs the positions and coordinates of all vertices
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);

	// Assigns the colors from the VertexData to ""color""
	color = aColor;
	// Assigns the texture coordinates from the vertex data to "texCoord"
	texCoord = aTex;
};