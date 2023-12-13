#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <thread>

#include <GL/glew.h>

#include <IndexBuffer.h>
#include <VertexArray.h>
#include <VertexBufferLayout.h>
#include <Shader.h>
#include <Texture.h>
#include <Display.h>

/* Thread task to get input from command line */
void getInputFromCommandLine(Display& display)
{
	while (true) {
		std::string input;
		// unsigned char AppID;
		unsigned char* data;

		// std::cout << "Enter a AppID: ";
		// std::getline(std::cin, input);
		// AppID = std::stoi(input);

		// std::cout << "Enter Data: ";
		// std::getline(std::cin, input);

		// data = new unsigned char[input.length()];
		// memcpy(data, input.c_str(), input.length());
		// display.CreateFormat(AppID, data, input.length());

		/* Get File name from the command line */
		std::cout << "Enter File name: ";
		std::getline(std::cin, input);

		/* Open file and create format */
		std::ifstream file;
		file.open(input, std::ios::in | std::ios::binary);
		file.seekg(0, std::ios::end);
		int length = file.tellg();
		file.seekg(0, std::ios::beg);

		data = new unsigned char[length];
		file.read((char*)data, length);
		file.close();
		std::cout << "File length: " << length << std::endl;

		display.CreateFormat(0x56, data, length);
	}
}

int main(int argc, char** argv)
{
	GLFWwindow* window;
	GLFWmonitor** monitors;
	int count, i;
	std::string display_name;

	// Get display name from the command line
	if (argc > 1) {
		display_name = argv[1];
	} else {
		display_name = "eDP-1";
	}

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hide the window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE); // Prevent the window from being minimized
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Remove the border and title bar

	// Get the primary monitor
	monitors = glfwGetMonitors(&count);
	for (i = 0; i < count; i++) {
		if (strcmp(glfwGetMonitorName(monitors[i]), display_name.c_str()) == 0) {
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

	std::cout << glGetString(GL_VERSION) << std::endl;

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

	Shader shader("./res/shader/VertShader.shader", "./res/shader/FragShader.shader");
	shader.Bind();

	Texture texture("./res/textures/texture.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;
	Display display;

	unsigned char* data = NULL;
	unsigned int data_length;
	bool dataSent = false;

	std::thread getInputThread(getInputFromCommandLine, std::ref(display));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		shader.Bind();

		renderer.Draw(va, ib, shader);

		if (!display.IsEmpty() && display.GetSize() >= 24) {
			renderer.Draw(va, ib, shader);

			data_length = display.GetSize();
			data = display.GetFormattedBuffer();

			/* Store the data in to the File */
			// std::ofstream file;
			// file.open("data.txt", std::ios::out | std::ios::binary);
			// file.write((char*)data, data_length);
			// file.close();

			texture.SetTexture(data, data_length);
			texture.Bind();

			dataSent = true;
		} else if (dataSent) {
			texture.Unbind();

			dataSent = false;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete data;
	glfwTerminate();
	return 0;
}

