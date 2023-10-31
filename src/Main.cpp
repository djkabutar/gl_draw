#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#include <GL/glew.h>

#include <IndexBuffer.h>
#include <VertexArray.h>
#include <VertexBufferLayout.h>
#include <Shader.h>
#include <Texture.h>

int main(void)
{
	GLFWwindow* window;
	GLFWmonitor** monitors;
	int count, i;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hide the window

	// Get the primary monitor
	monitors = glfwGetMonitors(&count);
	for (i = 0; i < count; i++) {
		if (strcmp(glfwGetMonitorName(monitors[i]), "eDP") == 0) {
			glfwWindowHint(GLFW_REFRESH_RATE, glfwGetVideoMode(monitors[i])->refreshRate);
			break;
		}
	}

	const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(mode->width, mode->height, "Hello World", monitors[i], NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
		-1.0f, -1.0f, 0.0f, 0.0f, // 0
		1.0f,  1.0f, 1.0f, 1.0f, // 1
		1.0f, -1.0f, 1.0f, 0.0f, // 2
		-1.0f,  1.0f, 0.0f, 1.0f // 3 
	};

	unsigned int indices[] = {
		0, 2, 1,
		1, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	unsigned int vao; // Vertex array object
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray va;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.AddFloat(2);
	layout.AddFloat(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	Shader shader("./res/shader/basic.shader");
	shader.Bind();

	shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

	Texture texture("./res/textures/texture.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	// Get the location of the uniform
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float r_increment = 0.05f;
	float g_increment = 0.03f;
	float b_increment = 0.04f;

	int count_frames = 0;
	int data_count = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		shader.Bind();
		shader.SetUniform4f("u_Color", r, g, b, 1.0f);

		renderer.Draw(va, ib, shader);

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

		count_frames++;

		if (count_frames == 60) {
			count_frames = 0;

			const std::string& data = std::to_string(data_count++);
			texture.SetTexture(data, data.length());
			texture.Bind();
		} else {
			texture.Unbind();
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
