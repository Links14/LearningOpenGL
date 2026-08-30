#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = R"glsl(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
};
)glsl";
const char* fragmentShaderSource = R"glsl(
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.7f, 0.3f, 0.02f, 1.0f);
};
)glsl";

int main() {
	// Initialize GLFW
	glfwInit();

	// inform opengl of target version (vMAJOR,MINOR = v3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use CORE profiler rather than COMPATABILITY, CORE drops the outdated functions present in COMPATABILITY
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat verticies[] = 
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
		0.5f, -0.5f, // lower right corner
		-0.5f, -0.5f, // lower left corner
		-0.5f, 0.5f, // upper left corner
		0.5f, 0.5f // upper right corner
	};
	// shape rendering parameters
	int vertPerPt = 2;
	int ptsPerShape = 4;
	int polygons = 1;

	// create window
	int windowWidth {800};
	int windowHeight {800};
	int asdad {1};
	const char windowName[] {"OpenGL Test"};
	// Create window with dimensions of (windowHeight, windowWidth) and a name
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	// ensure valid window object
	if (window == NULL) {
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

	// Create vertex shader and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach vertex shader source to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the vertex shader into machine code
	glCompileShader(vertexShader);

	// Create fragment shader object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source to the fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the vertex shader into machine code
	glCompileShader(fragmentShader);

	// Create shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();

	// Attach the vertex and fragment shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the sahder together in the shaderProgram
	glLinkProgram(shaderProgram);

	// Delete the shaders that are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create reference containers for the Vertex Array Object and the Vertex Buffer Object
	//GLuint VAO, VBO;
	GLuint VAOs[1], VBOs[1];

	// Generate the VAO and BVO with only 1 object each - VAO must be made first
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAOs[0]);
	// Bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

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

	// Introduce the verticies in to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, vertPerPt, GL_FLOAT, GL_FALSE, vertPerPt * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows how to use it
	glEnableVertexAttribArray(0);

	// Bind both VBO and VAO to 0 so that we dont accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAOs[0]);
		// Draw the triangle using the GL_TRIANGLES primitive
		//glDrawArrays(GL_TRIANGLES, 0, vertCount * trisCount);
		glDrawArrays(GL_LINE_LOOP, 0, ptsPerShape * polygons);
		// Updates frames
		glfwSwapBuffers(window);

		// process events
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteProgram(shaderProgram);

	// Delete and terminate window
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}