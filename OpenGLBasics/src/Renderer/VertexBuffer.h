#pragma once
namespace glb {

	class VertexBuffer {

	private:
		unsigned int m_RendererID;

	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		static VertexBuffer* Create(const void* data, unsigned int size);
	};
}