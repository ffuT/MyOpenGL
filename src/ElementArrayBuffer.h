#pragma once
class ElementArrayBuffer{
private:
	unsigned m_ID = 0;

public:

	ElementArrayBuffer(unsigned int size, const void* data);
	~ElementArrayBuffer();

	void Bind() const;
	void Unbind() const;

};

