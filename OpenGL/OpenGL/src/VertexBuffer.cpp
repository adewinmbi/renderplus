#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &m_RendererID); // Create vertex buffer, returns ID
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // Binding buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // Writing to buffer with vertex positions
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID))
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}