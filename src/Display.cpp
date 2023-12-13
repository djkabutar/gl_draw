#include <iostream>

#include <Display.h>

struct PreProcessedBuffer {
	unsigned char* buffer;
	unsigned int bufferSize;
};

Display::Display() {
}

Display::~Display() {
	clear();
}

unsigned char GetMask(unsigned char rem) {
	char mask = 0;

	switch(rem) {
		case 1:
			mask = 0b1;
			break;
		case 2:
			mask = 0b11;
			break;
		case 3:
			mask = 0b111;
			break;
		case 4:
			mask = 0b1111;
			break;
		case 5:
			mask = 0b11111;
			break;
		default:
			mask = 0b111111;
			break;
	}

	return mask;
}

void Display::StartOfFrame() {
	m_displayBuffer.push(0xEA);
	m_displayBuffer.push(0xFF);
	m_displayBuffer.push(0x99);
	m_displayBuffer.push(0xDE);
	m_displayBuffer.push(0xAD);
	m_displayBuffer.push(0xFF);
}

void Display::EndOfFrame() {
	m_displayBuffer.push(0xEA);
	m_displayBuffer.push(0xFF);
	m_displayBuffer.push(0x99);
	m_displayBuffer.push(0xDE);
	m_displayBuffer.push(0xAD);
	m_displayBuffer.push(0xAA);
}

void Display::AddHeader(unsigned char AppID, unsigned int bufferSize, unsigned char rem) {
	unsigned int packetSize = rem ? (bufferSize / 6) + 1 : bufferSize / 6;
	unsigned int temp = 0;

	/* The AppID is offset by 1
	 * The AppID 0x3F is reserved for the bootloader
	 */
	if (AppID < 0x3F) {
		AppID++;
	}
	m_displayBuffer.push(AppID);

	/* The packet size is offset by 1
	 * The packet size is the number of packets in the frame
	 */
	temp = (packetSize >> 16) & 0xFF;
	if (temp) {
		if (temp < 0x3F) {
			temp++;
		}
		m_displayBuffer.push(temp);
	} else {
		m_displayBuffer.push(0x00);
	}

	temp = (packetSize >> 8) & 0xFF;
	if (temp) {
		if (temp < 0x3F) {
			temp++;
		}
		m_displayBuffer.push(temp);
	} else {
		m_displayBuffer.push(0x00);
	}

	temp = packetSize & 0xFF;
	if (temp) {
		if (temp < 0x3F) {
			temp++;
		}
		m_displayBuffer.push(temp);
	} else {
		m_displayBuffer.push(0x00);
	}

	m_displayBuffer.push(GetMask(rem));

	m_displayBuffer.push(0x00);
}

void Display::clear() {
	while (!m_displayBuffer.empty()) {
		m_displayBuffer.pop();
	}
}

struct PreProcessedBuffer PreProcessingBuffer(unsigned char* displayBuffer, unsigned int bufferSize) {
	unsigned int escapeCount = 0;
	for (unsigned int i = 0; i < bufferSize; i++) {
		if (displayBuffer[i] == 0x3F || displayBuffer[i] == 0x5C) {
			escapeCount++;
		}
	}

	/* Allocate memory for the buffer
	 * The buffer size is the size of the original buffer plus the number of escape characters
	 * The escape characters are 0x3F and 0x5C
	 * This is because lesser than 0x40 characters are being offset by 1
	 */
	unsigned char* buffer = new unsigned char[bufferSize + escapeCount];
	unsigned int j = 0;
	for (unsigned int i = 0; i < bufferSize; i++) {
		if (displayBuffer[i] < 0x3F) {
			buffer[j] = displayBuffer[i] + 1;
		} else if (displayBuffer[i] == 0x3F) {
			buffer[j++] = 0x5C;
			buffer[j] = 0x3F;
		} else if (displayBuffer[i] == 0x5C) {
			buffer[j++] = 0x5C;
			buffer[j] = 0x5C;
		} else {
			buffer[j] = displayBuffer[i];
		}
		j++;
	}

	return { buffer, bufferSize + escapeCount };
}

void Display::CreateFormat(unsigned char AppID, unsigned char* displayBuffer, unsigned int bufferSize) {
	/* Do pre-processing on the buffer
	 * This is to escape the characters 0x3F and 0x5C
	 * 0x3F is escaped as 0x5C 0x3F
	 * 0x5C is escaped as 0x5C 0x5C
	 */
	struct PreProcessedBuffer preProcessedBuffer = PreProcessingBuffer(displayBuffer, bufferSize);
	unsigned char rem = preProcessedBuffer.bufferSize % 6;

	StartOfFrame();
	AddHeader(AppID, preProcessedBuffer.bufferSize, rem);

	for (unsigned int i = 0; i < preProcessedBuffer.bufferSize - rem; i++) {
		m_displayBuffer.push(preProcessedBuffer.buffer[i]);
	}

	if (rem) {
		for (unsigned int i = 0; i < 6 - rem; i++) {
			m_displayBuffer.push(0x00);
		}

		for (unsigned int i = preProcessedBuffer.bufferSize - rem; i < preProcessedBuffer.bufferSize; i++) {
			m_displayBuffer.push(preProcessedBuffer.buffer[i]);
		}
	}

	EndOfFrame();
}

unsigned char* Display::GetFormattedBuffer() {
	unsigned int bufferSize = m_displayBuffer.size();

	// Allocate memory for the buffer
	unsigned char* buffer = new unsigned char[bufferSize];
	for (unsigned int i = 0; i < bufferSize; i++) {
		buffer[i] = m_displayBuffer.front();
		m_displayBuffer.pop();
	}

	return buffer;
}

void Display::DisplayBuffer() {
	while (!m_displayBuffer.empty()) {
		std::cout << m_displayBuffer.front();
		m_displayBuffer.pop();
	}
	std::cout << std::endl;
}
