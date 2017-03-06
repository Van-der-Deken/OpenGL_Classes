//
// Created by Y500 on 06.03.2017.
//

#ifndef CONVERTER_GLBUFFER_H
#define CONVERTER_GLBUFFER_H

#include <glew.h>
#include <string>
#include <ostream>
#include <iostream>

struct GLBUFFER_INFO
{
    GLuint handle = 0;
    GLenum type = GL_BUFFER;
    GLenum usage = GL_BUFFER_USAGE;
    GLsizeiptr size = 0;
    GLuint baseIndex = 0;
    bool mapped = false;
    bool binded = false;
    bool bindedBase = false;

    std::string toString();
};

class GLBuffer {
    public:
        GLBuffer():errorStream(std::cerr.rdbuf()){};
        GLBuffer(GLenum type, const std::ostream &inErrorStream);
        ~GLBuffer();
        void setType(GLenum type);
        void setErrorStream(const std::ostream &inErrorStream);
        GLenum data(GLsizeiptr size, const void *data, GLenum usage);
        GLenum subData(GLintptr offset, GLsizeiptr size, const void *data);
        GLenum getSubData(GLintptr offset, GLsizeiptr size, void *data);
        void *map(GLenum access);
        void unmap();
        void *mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
        void bindBase(GLuint index);
        void bind();
        void unbind();
        GLBUFFER_INFO getBufferInfo();
    private:
        GLBUFFER_INFO buffer;
        std::ostream errorStream;
};


#endif //CONVERTER_GLBUFFER_H
