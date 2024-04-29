#pragma once

#include <vector>
#include <stdexcept>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferAttribute {

	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetTypeSize(unsigned int type) {
		switch (type) {

			case GL_FLOAT:          return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:

	std::vector<VertexBufferAttribute> m_Attributes;
	unsigned int m_Stride;
	

public:

	VertexBufferLayout()
		: m_Stride(0) {}


	template <typename T>
	void Push(unsigned int count) {
		std::runtime_error(false);
	}

	template <>
	void Push<float>(unsigned int count) {
		m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetTypeSize(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count) {
		m_Attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetTypeSize(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count) {
		m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferAttribute::GetTypeSize(GL_UNSIGNED_BYTE);
	}

	inline std::vector<VertexBufferAttribute> GetAttributes() const { return m_Attributes; }

	inline unsigned int GetStride() const { return m_Stride; }

};

