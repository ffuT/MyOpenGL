#pragma once

class ElementArrayBuffer{
public:
	ElementArrayBuffer(const unsigned int size, const void* data);
	~ElementArrayBuffer();

	void Bind() const;
	void Unbind() const;

private:
	unsigned m_ID = 0;
};

