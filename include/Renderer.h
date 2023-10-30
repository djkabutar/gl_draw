#include <GLFW/glfw3.h>

#include <csignal>

#define FRAME_SIZE 1280*1024*3

class VertexArray;
class IndexBuffer;
class Shader;

#define ASSERT(x) if (!(x)) std::raise(SIGTRAP); // Macro for debugging
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // Macro for debugging

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
