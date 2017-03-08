//
// Created by Y500 on 06.03.2017.
//

#ifndef GLBUFFER_H
#define GLBUFFER_H

#include <glew.h>
#include <string>
#include <ostream>
#include <iostream>

struct GLBUFFER_INFO
{
    GLuint handle = 0;
    GLenum type = 0;
    GLenum usage = 0;
    GLenum access = 0;
    GLbitfield accessBit = 0;
    GLsizeiptr size = 0;
    GLuint baseIndex = 0;
    bool mapped = false;
    bool boundBase = false;

    std::string toString() const;
};

class GLBuffer {
    public:
        GLBuffer():errorStream(std::cerr.rdbuf()){glGenBuffers(1, &buffer.handle);};
        GLBuffer(GLenum type, const std::ostream &inErrorStream = std::cerr);
        GLBuffer(const GLBuffer &origin);
        void setType(GLenum type);
        void setErrorStream(const std::ostream &inErrorStream);
        GLenum data(GLsizeiptr size, const void *data, GLenum usage);
        GLenum subData(GLintptr offset, GLsizeiptr size, const void *data);
        GLenum getSubData(GLintptr offset, GLsizeiptr size, void *data) const;
        void *map(GLenum access);
        GLboolean unmap();
        void *mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
        void bindBase(GLuint index);
        void bind();
        void unbind();
        void deleteBuffer();
        GLBUFFER_INFO getBufferInfo() const;
    private:
        GLBUFFER_INFO buffer;
        std::ostream errorStream;
};


#endif //GLBUFFER_H
