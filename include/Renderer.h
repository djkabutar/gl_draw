#include <GLFW/glfw3.h>

#include <csignal>

#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 1024
#define PIXEL_SIZE 3
#define FRAME_SIZE FRAME_WIDTH * FRAME_HEIGHT * PIXEL_SIZE

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
