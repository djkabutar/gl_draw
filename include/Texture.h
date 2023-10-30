#include <string>

class Texture
{
	private:
		unsigned int m_RendererID;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP; // BPP = Bits Per Pixel

	public:
		Texture(const std::string& frameBufferData);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		void SetTexture(const std::string& data, unsigned int length) const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
};
