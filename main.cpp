#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {

	glfwInit();

	// inform opengl of target version (vMAJOR,MINOR = v3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use CORE profiler rather than COMPATABILITY, CORE drops the outdated functions present in COMPATABILITY
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	int windowWidth{800};
	int windowHeight{800};
	// Create window with dimensions of (windowHeight, windowWidth) and a name
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "TestOpenGL", NULL, NULL);
	// ensure valid window object
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Add window to active context
	glfwMakeContextCurrent(window);

	// load GLAD to configure openGL
	gladLoadGL();

	// render the full window
	// origin at (0,0) in the bottom left to (windowWidth, windowHeight) in the top right corner
	glViewport(0, 0, windowWidth, windowHeight);

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
	// Use color for background
	glClearColor(R, G, B, A);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	// swap the back buffer with the front buffer
	glfwSwapBuffers(window);


	// only close on valid close case
	while (!glfwWindowShouldClose(window)) {
		// process events
		glfwPollEvents();
	}

	// Delete and terminate window
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
