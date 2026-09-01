#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
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

	// square-outline		(2, 4, 1)
	//0.5f, -0.5f, // lower right corner
	//-0.5f, -0.5f, // lower left corner
	//-0.5f, 0.5f, // upper left corner
	//0.5f, 0.5f // upper right corner

	// triforce
	-0.5f, -float(sqrt(3)) / 6, 0.0f, // lower left corner
	0.5f, -float(sqrt(3)) / 6, 0.0f, // lower right corner
	0.0f, float(sqrt(3)) / 3, 0.0f, // upper corner
	-0.25f, float(sqrt(3)) / 12, 0.0f, // Inner Left
	0.25f, float(sqrt(3)) / 12, 0.0f, // Inner Right
	0.0f, -float(sqrt(3)) / 6, 0.0f // Inner down
};

GLuint indices[] = {
	0, 3, 5,	// lower left triangle
	3, 2, 4,	// lower right triangle
	5, 4, 1		// Upper triangle
};

// shape rendering parameters
int vertPerPt = 3;
int ptsPerPolygon = 3;
int polygons = 3;

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

	// Link the VBO to the VAO
	VAO1.LinkVBO(VBO1, 0);
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

	// prepare to clear color of buffer and give it a new color
	// Specify Color
	float R{0.07f};
	float G{0.13f};
	float B{0.17f};
	float A{1.0f};

	// swap the back buffer with the front buffer
	glfwSwapBuffers(window);


	// only close on valid close case
	while (!glfwWindowShouldClose(window)) {

		// Use color for background
		glClearColor(R, G, B, A);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which shader program we want to use
		shaderProgram.Activate();
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
	shaderProgram.Delete();

	// Delete and terminate window
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}