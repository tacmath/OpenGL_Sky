#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "glad.h"
#include <cstddef>

class VBO
{
public:
    GLuint ID;

    VBO(const void* data, size_t size);
    void Gen(const void* data, size_t size);
    void Delete();
};

#endif