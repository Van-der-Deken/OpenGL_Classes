//
// Created by Y500 on 06.03.2017.
//

#include <sstream>
#include "GLBuffer.h"

std::string GLBUFFER_INFO::toString() const
{
    std::stringstream stringStream;
    stringStream << "Buffer info:\n"
                 << "\tHandle:" << handle
                 << "\n\tType:";
    switch (type)
    {
        case GL_ARRAY_BUFFER:
            stringStream << "Array buffer\n";
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            stringStream << "Atomic counter buffer\n";
            break;
        case GL_COPY_READ_BUFFER:
            stringStream << "Copy read buffer\n";
            break;
        case GL_COPY_WRITE_BUFFER:
            stringStream << "Copy write buffer\n";
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            stringStream << "Draw indirect buffer\n";
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            stringStream << "Dispatch indirect buffer\n";
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            stringStream << "Element array buffer\n";
            break;
        case GL_PIXEL_PACK_BUFFER:
            stringStream << "Pixel pack buffer\n";
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            stringStream << "Pixel unpack buffer\n";
            break;
        case GL_QUERY_BUFFER:
            stringStream << "Query buffer\n";
            break;
        case GL_SHADER_STORAGE_BUFFER:
            stringStream << "Shader storage buffer\n";
            break;
        case GL_TEXTURE_BUFFER:
            stringStream << "Texture buffer\n";
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            stringStream << "Transform feedback buffer\n";
            break;
        case GL_UNIFORM_BUFFER:
            stringStream << "Uniform buffer\n";
            break;
        default:
            stringStream << "Undefined\n";
    }
    stringStream << "\tUsage:";
    switch (usage)
    {
        case GL_STREAM_DRAW:
            stringStream << "Stream draw\n";
            break;
        case GL_STREAM_READ:
            stringStream << "Stream read\n";
            break;
        case GL_STREAM_COPY:
            stringStream << "Stream copy\n";
            break;
        case GL_STATIC_DRAW:
            stringStream << "Static draw\n";
            break;
        case GL_STATIC_READ:
            stringStream << "Static read\n";
            break;
        case GL_STATIC_COPY:
            stringStream << "Static copy\n";
            break;
        case GL_DYNAMIC_DRAW:
            stringStream << "Dynamic draw\n";
            break;
        case GL_DYNAMIC_READ:
            stringStream << "Dynamic read\n";
            break;
        case GL_DYNAMIC_COPY:
            stringStream << "Dynamic copy\n";
            break;
        default:
            stringStream << "Undefined\n";
    }
    stringStream << "\tAccess:";
    switch (access)
    {
        case GL_READ_ONLY:
            stringStream << "Read only\n";
            break;
        case GL_WRITE_ONLY:
            stringStream << "Write only\n";
            break;
        case GL_READ_WRITE:
            stringStream << "Read and write\n";
            break;
        default:
            stringStream << "Undefined\n";
    }
    stringStream << "\tSize:" << size << " bytes\n"
                 << "\tBase index:" << baseIndex
                 << "\n\tMapped:" << mapped
                 << "\n\tBound to base:" << boundBase << std::endl;
    return stringStream.str();
}

GLBuffer::GLBuffer(GLenum type, const std::ostream &inErrorStream) : errorStream(inErrorStream.rdbuf())
{
    glGenBuffers(1, &buffer.handle);
    buffer.type = type;
}

GLBuffer::GLBuffer(const GLBuffer &origin) : errorStream(origin.errorStream.rdbuf())
{
    buffer = origin.buffer;
}

void GLBuffer::setType(GLenum type)
{
    glGenBuffers(1, &buffer.handle);
    buffer.type = type;
}

void GLBuffer::setErrorStream(const std::ostream &inErrorStream)
{
    errorStream.rdbuf(inErrorStream.rdbuf());
}

GLenum GLBuffer::data(GLsizeiptr size, const void *data, GLenum usage)
{
    glBufferData(buffer.type, size, data, usage);
    GLenum error = glGetError();
    if(error == GL_NO_ERROR)
    {
        buffer.size = size;
        buffer.usage = usage;
        return GL_NO_ERROR;
    }
    else
        return error;
}

GLenum GLBuffer::subData(GLintptr offset, GLsizeiptr size, const void *data)
{
    glBufferSubData(buffer.type, offset, size, data);
    return glGetError();
}

GLenum GLBuffer::getSubData(GLintptr offset, GLsizeiptr size, void *data) const
{
    glGetBufferSubData(buffer.type, offset, size, data);
    return glGetError();
}

void* GLBuffer::map(GLenum access)
{
    buffer.access = access;
    buffer.mapped = true;
    return glMapBuffer(buffer.type, access);
}

GLboolean GLBuffer::unmap()
{
    buffer.access = GL_BUFFER_ACCESS;
    buffer.mapped = false;
    glUnmapBuffer(buffer.type);
}

void* GLBuffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    buffer.accessBit = access;
    buffer.mapped = true;
    return glMapBufferRange(buffer.type, offset, length, access);
}

void GLBuffer::bindBase(GLuint index)
{
    buffer.boundBase = true;
    buffer.baseIndex = index;
    glBindBufferBase(buffer.type, index, buffer.handle);
}

void GLBuffer::bind()
{
    glBindBuffer(buffer.type, buffer.handle);
}

void GLBuffer::unbind()
{
    glBindBuffer(buffer.type, 0);
}

void GLBuffer::deleteBuffer()
{
    if(buffer.handle != 0)
    {
        glDeleteBuffers(1, &buffer.handle);
        glBindBuffer(buffer.type, 0);
    }
}

GLBUFFER_INFO GLBuffer::getBufferInfo() const
{
    return buffer;
}