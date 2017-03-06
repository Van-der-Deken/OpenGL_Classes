//
// Created by Y500 on 03.10.2016.
//

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <vector>
#include <string>
#include <map>

struct SHADER_INFO
{
    GLuint handle = 0;
    std::string path = "";
    GLenum type = GL_SHADER;
    bool compiled = false;
    bool attached = false;

    std::string toString();
};

struct PROGRAM_INFO
{
    GLuint handle = 0;
    SHADER_INFO vertex;
    SHADER_INFO geometry;
    SHADER_INFO fragment;
    SHADER_INFO compute;
    bool linked = false;
    bool used = false;

    std::string toString();
};

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
        ShaderProgram():program(), errorStream(std::cerr.rdbuf())
            {errorLog.resize(1);};
        ShaderProgram(const std::ostream &inErrorStream):program(), errorStream(inErrorStream.rdbuf())
            {errorLog.resize(1);};
        ~ShaderProgram();
        void setErrorStream(const std::ostream &inErrorStream);
        bool loadShaderFromFile(const std::string &filePath, GLenum shaderType);
        bool loadShaderFromString(const std::string &source, GLenum shaderType);
        void bindUniform(const std::string &uniName, GLfloat v0);
        void bindUniformi(const std::string &uniName, GLint v0);
        void bindUniformui(const std::string &uniName, GLuint v0);
        void bindUniformVector(UTypes type, const std::string &uniName, const GLfloat *value, GLsizei count = 1);
        void bindUniformVector(UTypes type, const std::string &uniName, const GLuint *value, GLsizei count = 1);
        void bindUniformVector(UTypes type, const std::string &uniName, const GLint *value, GLsizei count = 1);
        void bindUniformMatrix(UTypes type, const std::string &uniName, const GLfloat* value,
                               GLboolean transpose = GL_TRUE, GLsizei count = 1);
        void bindAttributeData(const std::string &attribName, GLint size, GLenum type, GLboolean normalized,
                               GLsizei stride, const GLvoid* pointer);
        void disableAttribute(const std::string &name);
        bool link();
        void use();
        void unuse();
        void printError();
        PROGRAM_INFO getProgramInfo();
    private:
        std::string loadFile(const std::string &filePath);
        void assignUniform(const std::string &name);
        void assignAttribute(const std::string &name);

        PROGRAM_INFO program;
        std::ostream errorStream;
        std::vector<char> errorLog;
        std::map<std::string, GLint> uniforms;
        std::map<std::string, GLint> attributes;
};


#endif //SHADER_PROGRAM_H
