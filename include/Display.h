#include <queue>

class Display
{
	private:
		std::queue<unsigned char> m_displayBuffer;
		void StartOfFrame();
		void EndOfFrame();
		void AddHeader(unsigned char AppID, unsigned int bufferSize, unsigned char rem);
	public:
		Display();
		~Display();

		void clear();
		void CreateFormat(unsigned char AppID, unsigned char* displayBuffer, unsigned int bufferSize);
		unsigned char* GetFormattedBuffer();
		void DisplayBuffer();
		inline bool IsEmpty() { return m_displayBuffer.empty(); }
		inline unsigned int GetSize() { return m_displayBuffer.size(); }
};
