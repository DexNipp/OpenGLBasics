#include "VertexArray.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {

	GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {

	Bind();
	vb.Bind();
	const auto& attributes = layout.GetAttributes();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < attributes.size(); i++) {
		const auto& attribute = attributes[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized, layout.GetStride(), (const void*)offset);
		offset += attribute.count * VertexBufferAttribute::GetTypeSize(attribute.type);;
	}
}

void VertexArray::Bind() const {
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCALL(glBindVertexArray(0));
}
