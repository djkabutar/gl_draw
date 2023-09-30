#include <iostream>

#include <Renderer.h>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR); // Clear all errors
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) { // Get the error code
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

