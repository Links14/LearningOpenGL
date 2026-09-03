#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "shaderClass.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

GLfloat vertices[] =
{
	// equilateral triangle		(3, 3, 1)
	//-0.5f, -float(sqrt(3)) / 6.0f, 0.0f, // lower left corner
	//0.5f, -float(sqrt(3)) / 6.0f, 0.0f, // lower right corner
	//0.0f, float(sqrt(3)) / 3.0f, 0.0f // top

	// square, 2 tris			(3, 3, 2)
	//-0.5f, -0.5f, 0.0f, // lower left corner
	//0.5f, -0.5f, 0.0f, // lower right corner
	//-0.5f, 0.5f, 0.0f, // upper left corner

	//-0.5f, 0.5f, 0.0f, // upper left corner
	//0.5f, 0.5f, 0.0f, // upper right corner
	//0.5f, -0.5f, 0.0f, // lower right corner

	// square
	// vertices				// colors			// tex coords
	-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower Left corner
	-0.5f,	0.5f,	0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper Left corner
	0.5f,	0.5f,	0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper Right corner
	0.5f,	-0.5f,	0.0f,	1.0f, 1.0f, 1.0f, 	1.0f, 0.0f // Lower Right corner

	// triforce
	//-0.5f,	-float(sqrt(3)) / 6,	0.0f,	0.0f, 0.0f, 1.0f, // Blue		// Lower Left
	//0.5f,	-float(sqrt(3)) / 6,	0.0f,	0.0f, 1.0f, 0.0f, // Green		// Lower Right
	//0.0f,	 float(sqrt(3)) / 3,	0.0f,	1.0f, 0.0f, 0.0f, // Red		// Top Middle
	//-0.25f,	 float(sqrt(3)) / 12,	0.0f,	1.0f, 0.0f, 1.0f, // Magenta	// Middle Left
	//0.25f,	 float(sqrt(3)) / 12,	0.0f,	1.0f, 1.0f, 0.0f, // Yellow		// Middle Right
	//0.0f,	-float(sqrt(3)) / 6,	0.0f,	0.0f, 1.0f, 1.0f  // LightBlue	// Bottom Middle
};

// clockwise winding order
GLuint indices[] = {
	// triforce
	//0, 3, 5,	// lower left triangle
	//3, 2, 4,	// lower right triangle
	//5, 4, 1		// Upper triangle

	// square
	0, 2, 1,
	0, 3, 2
};

// shape rendering parameters
int vertPerPt = 3;
int ptsPerPolygon = 3;
int polygons = 2;

// create window
int windowWidth{800};
int windowHeight{800};
int asdad{1};
const char windowName[]{"OpenGL Test"};


int main()
{
// Initialize GLFW
	glfwInit();

	// inform opengl of target version (vMAJOR,MINOR = v3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use CORE profiler rather than COMPATABILITY, CORE drops the outdated functions present in COMPATABILITY
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create window with dimensions of (windowHeight, windowWidth) and a name
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	// ensure valid window object
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Add window to active context
	glfwMakeContextCurrent(window);

	// Load GLAD to configure openGL
	gladLoadGL();

	// Render the full window
	// Origin at (0,0) in the bottom left to (windowWidth, windowHeight) in the top right corner
	glViewport(0, 0, windowWidth, windowHeight);

	// creates shader object using shaders default.vert and default.frag
	Shader shaderProgram {"default.vert", "default.frag"};

	// create and bind a Vertex Array Object
	VAO VAO1;
	VAO1.Bind();

	// Create and Initialize Vertex Buffer Object and Element Array Buffer Object
	VBO VBO1{vertices, sizeof(vertices)};	// linked to vertices
	EBO EBO1{indices, sizeof(indices)};		// linked to indices

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// unbind to prevent accidental modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	/*
	* NOTE: 
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	* Rendering renders each pixel in a frame before displaying that frame.
	* Bufffer space is used to draw pixels to the frame up to the point that the frame is displayed.
	* Once its displayed the buffer begin writing new frame data.
	* The frame where data is being read from to display to the screen is the front buffer.
	* The frame where data is being written is called the back buffer
	*/

	// Get ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture
	// create sun texture
	Texture sun{"sun.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, STBI_rgb_alpha};
	// create and bind uniform texture to shader
	sun.texUnit(shaderProgram, "tex0", 0);

	// prepare to clear color of buffer and give it a new color
	// Specify Color
	float R{0.07f};
	float G{0.13f};
	float B{0.17f};
	float A{1.0f};

	// swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// only close on valid close case
	while (!glfwWindowShouldClose(window))
	{
		// Use color for background
		glClearColor(R, G, B, A);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which shader program we want to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activatin the Shader Program
		glUniform1f(uniID, 0.5f);
		sun.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		//glDrawArrays(GL_TRIANGLES, 0, vertPerPt * polygons);
		//glDrawArrays(GL_LINE_LOOP, 0, ptsPerShape * polygons);
		glDrawElements(GL_TRIANGLES, vertPerPt * polygons, GL_UNSIGNED_INT, 0);
		// Updates frames
		glfwSwapBuffers(window);

		// process events
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	sun.Delete();
	shaderProgram.Delete();

	// Delete and terminate window
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}