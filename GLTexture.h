//
// Created by Y500 on 07.03.2017.
//

#ifndef CONVERTER_GLTEXTURE_H
#define CONVERTER_GLTEXTURE_H

#include <GL/glew.h>
#include <string>
#include <ostream>
#include <iostream>

struct GLTEXTURE_INFO
{
    GLuint handle = 0;
    GLenum type = 0;
    GLsizei width = 0;
    GLsizei height = 0;
    GLsizei depth = 0;
    GLint internalFormat = 0;
    GLenum format = 0;
    GLenum valueType = 0;
    GLuint imageUnit = 0;
    GLenum access = 0;
    bool boundAsImage = false;

    std::string toString();
};

class GLTexture {
    public:
        GLTexture() : errorStream(std::cerr.rdbuf()){glGenTextures(1, &texture.handle);};
        GLTexture(GLenum type, const std::ostream &inErrorStream = std::cerr);
        void setType(GLenum type);
        void setErrorStream(const std::ostream &inErrorStream);
        void specifyImage(GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format,
                          GLenum type, const GLvoid *data);
        void specifyImage(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border,
                          GLenum format, GLenum type, const GLvoid *data);
        void specifyImage(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth,
                          GLint border, GLenum format, GLenum type, const GLvoid *data);
        void parameter(GLenum pname, GLint param);
        void parameter(GLenum pname, GLfloat param);
        void subImage(GLint level, GLint xoffset, GLsizei width, const GLvoid *data);
        void subImage(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const GLvoid *data);
        void subImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
                      GLsizei depth, const GLvoid *data);
        void bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
        void getSubImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
                         GLsizei depth, GLsizei bufSize, void *pixels);
        void bind();
        void unbind();
        void deleteTexture();
        GLTEXTURE_INFO getTextureInfo();
    private:
        GLTEXTURE_INFO texture;
        std::ostream errorStream;
};


#endif //CONVERTER_GLTEXTURE_H
