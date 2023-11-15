#include "VAO.h"

void VAO::LinkAttrib(const void* data, GLuint vertexNumbers, GLuint layout, GLuint numComponents, GLenum type, GLuint typeSize, GLuint offset) {
    GLuint VBO;

    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, numComponents * typeSize * vertexNumbers, data, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(ID, layout);
    glVertexArrayAttribBinding(ID, layout, 0);
    glVertexArrayAttribFormat(ID, layout, numComponents, type, GL_FALSE, offset);
    
    glVertexArrayVertexBuffer(ID, 0, VBO, 0, numComponents * typeSize);
}

void VAO::LinkAttrib(const VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLuint typeSize, GLuint offset) {

    glEnableVertexArrayAttrib(ID, layout);
    glVertexArrayAttribBinding(ID, layout, 0);
    glVertexArrayAttribFormat(ID, layout, numComponents, type, GL_FALSE, offset);

    glVertexArrayVertexBuffer(ID, 0, vbo.ID, 0, numComponents * typeSize); //TODO maybe remove typeSize and use type to determine the tipe size
}


void VAO::Gen() {
    glCreateVertexArrays(1, &ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}