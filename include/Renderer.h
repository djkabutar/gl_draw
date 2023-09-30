#include <GLFW/glfw3.h>

#include <csignal>

#define ASSERT(x) if (!(x)) std::raise(SIGTRAP); // Macro for debugging
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // Macro for debugging

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
