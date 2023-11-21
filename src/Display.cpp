#include <iostream>

#include <Display.h>

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
	m_displayBuffer.push(AppID);
	m_displayBuffer.push((packetSize >> 16) & 0xFF);
	m_displayBuffer.push((packetSize >> 8) & 0xFF);
	m_displayBuffer.push(packetSize & 0xFF);
	m_displayBuffer.push(GetMask(rem));
	m_displayBuffer.push(0x00);
}

void Display::clear() {
	while (!m_displayBuffer.empty()) {
		m_displayBuffer.pop();
	}
}

void Display::CreateFormat(unsigned char AppID, unsigned char* displayBuffer, unsigned int bufferSize) {
	unsigned char rem = bufferSize % 6;
	StartOfFrame();
	AddHeader(AppID, bufferSize, rem);

	for (unsigned int i = 0; i < bufferSize - rem; i++) {
		m_displayBuffer.push(displayBuffer[i]);
	}

	if (rem) {
		for (unsigned int i = 0; i < 6 - rem; i++) {
			m_displayBuffer.push(0x00);
		}

		for (unsigned int i = bufferSize - rem; i < bufferSize; i++) {
			m_displayBuffer.push(displayBuffer[i]);
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
		std::cout << "Buffer: 0x" << std::hex << (int)buffer[i] << std::endl;
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
