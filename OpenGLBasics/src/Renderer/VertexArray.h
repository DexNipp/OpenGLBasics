#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

namespace glb {

	class VertexArray
	{
	private:

		unsigned int m_RendererID;

	public:
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;

		static VertexArray* Create();
	};

}