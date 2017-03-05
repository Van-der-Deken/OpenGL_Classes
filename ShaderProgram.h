//
// Created by Y500 on 03.10.2016.
//

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H


#include <glew.h>
#include <vector>
#include <string>
#include <map>

enum UTypes
{
    SP_VEC2,
    SP_VEC3,
    SP_VEC4,
    SP_IVEC2,
    SP_IVEC3,
    SP_IVEC4,
    SP_UVEC2,
    SP_UVEC3,
    SP_UVEC4,
    SP_MAT2,
    SP_MAT3,
    SP_MAT4,
};

class ShaderProgram {
    public:
        ShaderProgram():program(0),vertex(0),fragment(0),geometry(0),compute(0), errorStream(std::cerr.rdbuf())
            {errorLog.resize(1);};
        ShaderProgram(std::ostream &inErrorStream):program(0),vertex(0),fragment(0),geometry(0),compute(0),
                                                   errorStream(inErrorStream.rdbuf())
            {errorLog.resize(1);};
        ~ShaderProgram();
        bool loadShaderFromFile(std::string filePath, GLenum shaderType);
        bool loadShaderFromString(std::string source, GLenum shaderType);
        void bindUniform(std::string uniName, GLfloat v0);
        void bindUniformi(std::string uniName, GLint v0);
        void bindUniformui(std::string uniName, GLuint v0);
        void bindUniformVector(UTypes type, std::string uniName, const GLfloat *value, GLsizei count = 1);
        void bindUniformVector(UTypes type, std::string uniName, const GLuint *value, GLsizei count = 1);
        void bindUniformVector(UTypes type, std::string uniName, const GLint *value, GLsizei count = 1);
        void bindUniformMatrix(UTypes type, std::string uniName, const GLfloat* value, GLboolean transpose = GL_TRUE, GLsizei count = 1);
        void bindAttributeData(std::string attribName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
        void disableAttribute(std::string name);
        bool link();
        void use();
        void unuse();
        void printError();
        GLuint getHandle();
    private:
        std::string loadFile(std::string filePath);
        void assignUniform(std::string name);
        void assignAttribute(std::string name);

        GLuint program;
        GLuint vertex;
        GLuint geometry;
        GLuint fragment;
        GLuint compute;
        std::ostream errorStream;
        std::vector<char> errorLog;
        std::map<std::string, GLint> uniforms;
        std::map<std::string, GLint> attributes;
};


#endif //SHADER_PROGRAM_H
