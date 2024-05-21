#pragma once
#include "Core/Asserts.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include <GL/glew.h>

#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:

    void Clear() const;
    void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawArrays(const VertexArray& va, const Shader& shader);
    
    void EnableDepthTest();
};

