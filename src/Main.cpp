#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

#include <Renderer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // Enable vsync

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	float positions[] = {
		-1.0f, -1.0f, // 0
		1.0f,  1.0f, // 1
		1.0f, -1.0f, // 2
		-1.0f,  1.0f  // 3 
	};

	unsigned int indices[] = {
		0, 2, 1,
		1, 3, 0
	};

	unsigned int vao; // Vertex array object
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray va;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.AddFloat(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));

	Shader shader("./res/shader/basic.shader");
	shader.Bind();

	shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	// Get the location of the uniform
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float r_increment = 0.05f;
	float g_increment = 0.03f;
	float b_increment = 0.04f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		shader.Bind();
		shader.SetUniform4f("u_Color", r, g, b, 1.0f);

		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 
					sizeof(indices) / sizeof(unsigned int), 
					GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f) {
			r_increment = -0.05f;
		} else if (r < 0.0f) {
			r_increment = 0.05f;
		}

		if (g > 1.0f) {
			g_increment = -0.03f;
		} else if (g < 0.0f) {
			g_increment = 0.03f;
		}

		if (b > 1.0f) {
			b_increment = -0.04f;
		} else if (b < 0.0f) {
			b_increment = 0.04f;
		}

		r += r_increment;
		g += g_increment;
		b += b_increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
