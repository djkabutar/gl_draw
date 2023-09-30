#include <vector>
#include <GL/glew.h>

#include <Renderer.h>

struct VertexBufferElement {
	GLuint type;
	GLuint count;
	GLboolean normalized;

	static GLuint GetSizeOfType(GLuint type) {
		switch(type) {
			case GL_FLOAT: return sizeof(GLfloat);
			case GL_UNSIGNED_INT: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> m_elements;
		GLuint m_stride;

		void Push(GLuint type, GLuint count, GLboolean normalized) {
			m_elements.push_back({type, count, normalized});
			m_stride += count * VertexBufferElement::GetSizeOfType(type);
		}
	public:
		VertexBufferLayout() : m_stride(0) {}

		void AddFloat(GLuint count) {
			Push(GL_FLOAT, count, GL_FALSE);
		}

		void AddUnsignedInt(GLuint count) {
			Push(GL_UNSIGNED_INT, count, GL_FALSE);
		}

		void AddUnsignedByte(GLuint count) {
			Push(GL_UNSIGNED_BYTE, count, GL_TRUE);
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
		inline GLuint GetStride() const { return m_stride; }
};
