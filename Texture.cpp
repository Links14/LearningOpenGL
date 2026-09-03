#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, GLuint desiredChannels)
{
	type = texType;

	// Stores the width, height, and number of color channels in the image
	int widthImg, heightImg, numColCh;
	// flips the image after load to correct the differences between stb and opengl, this makes the image upright
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, desiredChannels);

	// Generates and OpenGL Texture Object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(slot);
	Texture::Bind();

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the image repeats if at all
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra line if deciding to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to OpenGL Texture Object
	// rgb for jpg, rgba for png
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generates Mipmaps
	glGenerateMipmap(texType);

	// Deletes the image data as it is already in the OpenGL Texture Object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture Object so that it cant accidentally be modified
	Texture::Unbind();
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform;
	shader.Activate();
	// Set the value of the uniform
	glUniform1i(tex0Uni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
