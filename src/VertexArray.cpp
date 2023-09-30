#include <GL/glew.h>

#include <VertexArray.h>
#include <Renderer.h>

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind(); // Bind the vertex array
	vb.Bind(); // Bind the vertex buffer
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		// Enable the attribute
		GLCall(glEnableVertexAttribArray(i));
		// Specify the layout of the attribute
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		// Update the offset
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
