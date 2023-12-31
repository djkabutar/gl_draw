#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <Shader.h>
#include <Renderer.h>

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
	: m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(vertPath, fragPath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& vertPath, const std::string& fragPath) {
	std::ifstream vertStream(vertPath);
	std::ifstream fragStream(fragPath);

	std::stringstream ss[2]; // 0 = vertex, 1 = fragment

	ss[0] << vertStream.rdbuf();
	vertStream.close();

	ss[1] << fragStream.rdbuf();
	fragStream.close();

	return { ss[0].str(), ss[1].str() };
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // source is a std::string, so we need to get the raw pointer
	GLCall(glShaderSource(id, 1, &src, nullptr)); // 1 is the number of strings we're passing in
	GLCall(glCompileShader(id));

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // iv = integer vector
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // Get the length of the error message
		char* message = (char*)alloca(length * sizeof(char)); // Allocate memory on the stack
		GLCall(glGetShaderInfoLog(id, length, &length, message)); // Get the error message
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id)); // Delete the shader

		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs)); // Delete the intermediate shader files
	GLCall(glDeleteShader(fs)); // Delete the intermediate shader files

	return program;
}


void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		// If the uniform location is already in the cache, return it
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	// Add the uniform location to the cache
	m_UniformLocationCache[name] = location;
	return location;
}
