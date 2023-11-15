#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "glad.h"
#include <cstddef>

class VBO
{
public:
    GLuint ID;

    VBO() { ID = 0; };
    VBO(const void* data, size_t size);
    //* data = pointer to the data buffer
    //* size = the size of the buffer in bytes
    void Gen(const void* data, size_t size);
    void Delete();
};

#endif