//
// Created by Y500 on 07.03.2017.
//

#include <sstream>
#include "GLTexture.h"

std::string GLTEXTURE_INFO::toString()
{
    std::stringstream stringStream;
    stringStream << "Texture info:\n"
                 << "\tHandle:" << handle
                 << "\n\tType:";
    switch (type)
    {
        case GL_TEXTURE_1D:
            stringStream << "One dimensional texture\n";
            break;
        case GL_TEXTURE_2D:
            stringStream << "Two dimensional texture\n";
            break;
        case GL_TEXTURE_3D:
            stringStream << "Three dimensional texture\n";
            break;
        case GL_TEXTURE_1D_ARRAY:
            stringStream << "Array of one dimensional textures\n";
            break;
        case GL_TEXTURE_2D_ARRAY:
            stringStream << "Array of two dimensional textures\n";
            break;
        case GL_TEXTURE_RECTANGLE:
            stringStream << "Rectangle texture\n";
            break;
        case GL_TEXTURE_CUBE_MAP:
            stringStream << "Cube map texture\n";
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            stringStream << "Array of cube map textures\n";
            break;
        case GL_TEXTURE_BUFFER:
            stringStream << "Texture buffer\n";
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            stringStream << "Two dimensional texture multisample\n";
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            stringStream << "Array of two dimensional texture multisample\n";
            break;
        default:
            stringStream << "Undefined\n";
    }
    stringStream << "\tWidth:" << width
                 << "\n\tHeight:" << height
                 << "\n\tDepth:" << depth
                 << "\n\tInternal format:" << internalFormat
                 << "\n\tFormat:" << format
                 << "\n\tValue type:" << valueType
                 << "\n\tImage unit:" << imageUnit
                 << "\n\tAccess:";
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
    stringStream << "\tBound to image unit:" << boundAsImage << std::endl;
    return stringStream.str();
}

GLTexture::GLTexture(GLenum type, const std::ostream &inErrorStream) : errorStream(inErrorStream.rdbuf())
{
    glGenTextures(1, &texture.handle);
    texture.type = type;
}

void GLTexture::setType(GLenum type)
{
    texture.type = type;
}

void GLTexture::setErrorStream(const std::ostream &inErrorStream)
{
    errorStream.rdbuf(inErrorStream.rdbuf());
}

void GLTexture::specifyImage(GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format,
                             GLenum type, const GLvoid *data)
{
    glTexImage1D(texture.type, level, internalFormat, width, border, format, type, data);
    texture.internalFormat = internalFormat;
    texture.width = width;
    texture.format = format;
    texture.valueType = type;
}

void GLTexture::specifyImage(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border,
                             GLenum format, GLenum type, const GLvoid *data)
{
    glTexImage2D(texture.type, level, internalFormat, width, height, border, format, type, data);
    texture.internalFormat = internalFormat;
    texture.width = width;
    texture.height = height;
    texture.format = format;
    texture.valueType = type;
}

void GLTexture::specifyImage(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth,
                             GLint border, GLenum format, GLenum type, const GLvoid *data)
{
    glTexImage3D(texture.type, level, internalFormat, width, height, depth, border, format, type, data);
    texture.internalFormat = internalFormat;
    texture.width = width;
    texture.height = height;
    texture.depth = depth;
    texture.format = format;
    texture.valueType = type;
}

void GLTexture::parameter(GLenum pname, GLint param)
{
    glTexParameteri(texture.type, pname, param);
}

void GLTexture::parameter(GLenum pname, GLfloat param)
{
    glTexParameterf(texture.type, pname, param);
}

void GLTexture::subImage(GLint level, GLint xoffset, GLsizei width, const GLvoid *data)
{
    glTexSubImage1D(texture.type, level, xoffset, width, texture.format, texture.type, data);
}

void GLTexture::subImage(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
                         const GLvoid *data)
{
    glTexSubImage2D(texture.type, level, xoffset, yoffset, width, height, texture.format, texture.valueType, data);
}

void GLTexture::subImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
                         GLsizei depth, const GLvoid *data)
{
    glTexSubImage3D(texture.type, level, xoffset, yoffset, zoffset, width, height, depth, texture.format,
                    texture.type, data);
}

void GLTexture::bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
    glBindImageTexture(unit, texture.handle, level, layered, layer, access, format);
    texture.imageUnit = unit;
    texture.access = access;
    texture.boundAsImage = true;
}

void GLTexture::getSubImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
                            GLsizei depth, GLsizei bufSize, void *pixels)
{
    glGetTextureSubImage(texture.handle, level, xoffset, yoffset, zoffset, width, height, depth, texture.format,
                         texture.valueType, bufSize, pixels);
}

void GLTexture::bind()
{
    glBindTexture(texture.type, texture.handle);
}

void GLTexture::unbind()
{
    glBindTexture(texture.type, 0);
}

void GLTexture::deleteTexture()
{
    glDeleteTextures(1, &texture.handle);
    glBindTexture(texture.type, 0);
    GLTEXTURE_INFO empty;
    texture = empty;
}

GLTEXTURE_INFO GLTexture::getTextureInfo()
{
    return texture;
}