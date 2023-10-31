#include <iostream>
#include <cstdlib>
#include <cstring>

#include <Texture.h>
#include <Renderer.h>

Texture::Texture(const std::string& frameBufferData)
	: m_RendererID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0)
{
	m_LocalBuffer = new unsigned char[FRAME_SIZE];
	memset(m_LocalBuffer, 0x00, FRAME_SIZE);

	m_Width = 1920;
	m_Height = 1200;

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));

	if (m_LocalBuffer)
		delete[] m_LocalBuffer;
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetTexture(const std::string& data, unsigned int length) const
{
	memset(m_LocalBuffer, 0x00, FRAME_SIZE);
	memcpy(m_LocalBuffer, data.c_str(), length);
	std::cout << "Data: " << data.c_str() << std::endl;

	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
