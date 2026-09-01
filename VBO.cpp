#include"VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	// Bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// Introduce the verticies in to the VBO
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	/*
	* GLenum is GL_STATIC_DRAW
	*	- STREAM is used for when the verticies are modified once and used a few times
	*	- STATIC is used for when the verticies are modified once and used many times
	*	- DYNAMIC is used for when the verticies are modified once and use many times
	*	knowing these is useful for optimization
	*	DRAW is used to draw to the screen
	*	READ reads the info
	*	COPY copies it
	*/
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}