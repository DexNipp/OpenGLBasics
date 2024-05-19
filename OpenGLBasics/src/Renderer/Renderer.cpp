#include <iostream>
#include "Renderer.h"


void GLClearError() {

    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {

    while (GLenum error = glGetError()) {
        std::cout << "OpenGL ERROR: (" << error << "): " << function << " " << file << " LINE: " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawArrays(const VertexArray& va, const Shader& shader) {
    shader.Bind();
    va.Bind();
    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36)); // Testing for cube***
}
