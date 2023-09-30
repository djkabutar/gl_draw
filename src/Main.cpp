#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

#include <Renderer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2]; // 0 = vertex, 1 = fragment
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				// Set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				// Set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			// Add line to appropriate shader
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}


static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // source is a std::string, so we need to get the raw pointer
	glShaderSource(id, 1, &src, nullptr); // 1 is the number of strings we're passing in
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // iv = integer vector
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Get the length of the error message
		char* message = (char*)alloca(length * sizeof(char)); // Allocate memory on the stack
		glGetShaderInfoLog(id, length, &length, message); // Get the error message
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id); // Delete the shader

		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs); // Delete the intermediate shader files
	glDeleteShader(fs); // Delete the intermediate shader files

	return program;
}

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

	ShaderProgramSource source = ParseShader("./res/shader/basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	// Get the location of the uniform
	int location = glGetUniformLocation(shader, "u_Color");
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

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

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, g, b, 1.0f));

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

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
