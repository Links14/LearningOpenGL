#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	// Build ShaderProgram from two different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
private:
	// checks if the different shaders have comiled properly
	void compileErrors(unsigned int shader, const char* type);
};

#endif
