//
// Created by Y500 on 03.10.2016.
//

#include <iostream>
#include <fstream>
#include "ShaderProgram.h"

ShaderProgram::~ShaderProgram()
{
    if(vertex != 0)
        glDeleteShader(vertex);
    if(geometry != 0)
        glDeleteShader(geometry);
    if(fragment != 0)
        glDeleteShader(fragment);
    if(compute != 0)
        glDeleteShader(compute);
    if(program != 0)
        glDeleteProgram(program);
    glUseProgram(0);
}

bool ShaderProgram::loadShaderFromFile(std::string filePath, GLenum shaderType)
{
    std::string source = loadFile(filePath);
    if(source.empty())
        return false;
    return loadShaderFromString(source, shaderType);
}

bool ShaderProgram::loadShaderFromString(std::string source, GLenum shaderType)
{
    const char* cSource = source.c_str();
    GLint result = GL_FALSE;
    int logLength;
    switch (shaderType)
    {
        case GL_VERTEX_SHADER:
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &cSource, NULL);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
            glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &logLength);
            errorLog.resize(logLength);
            glGetShaderInfoLog(vertex, logLength, NULL, &errorLog[0]);
            break;
        case GL_FRAGMENT_SHADER:
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &cSource, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
            glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &logLength);
            errorLog.resize(logLength);
            glGetShaderInfoLog(fragment, logLength, NULL, &errorLog[0]);
            break;
        case GL_GEOMETRY_SHADER:
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &cSource, NULL);
            glCompileShader(geometry);
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &result);
            glGetShaderiv(geometry, GL_INFO_LOG_LENGTH, &logLength);
            errorLog.resize(logLength);
            glGetShaderInfoLog(geometry, logLength, NULL, &errorLog[0]);
            break;
        case GL_COMPUTE_SHADER:
            compute = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(compute, 1, &cSource, NULL);
            glCompileShader(compute);
            glGetShaderiv(compute, GL_COMPILE_STATUS, &result);
            glGetShaderiv(compute, GL_INFO_LOG_LENGTH, &logLength);
            errorLog.resize(logLength);
            glGetShaderInfoLog(compute, logLength, NULL, &errorLog[0]);
            break;
        default:
            errorStream << shaderType << ":unsupported type of shader\n";
            return false;
    }
    return result == GL_TRUE;
}

void ShaderProgram::bindUniform(std::string uniName, GLfloat v0)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    glUniform1f(uniforms[uniName], v0);
}

void ShaderProgram::bindUniformi(std::string uniName, int v0)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    glUniform1i(uniforms[uniName], v0);
}

void ShaderProgram::bindUniformui(std::string uniName, uint32_t v0)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    glUniform1ui(uniforms[uniName], v0);
}

void ShaderProgram::bindUniformVector(UTypes type, std::string uniName, const GLfloat *value, GLsizei count)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    switch (type)
    {
        case SP_VEC2:
            glUniform2fv(uniforms[uniName], count, value);
            break;
        case SP_VEC3:
            glUniform3fv(uniforms[uniName], count, value);
            break;
        case SP_VEC4:
            glUniform4fv(uniforms[uniName], count, value);
            break;
        default:
            errorStream << "Wrong type of uniform\n";
    }
}

void ShaderProgram::bindUniformVector(UTypes type, std::string uniName, const GLuint *value, GLsizei count)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    switch (type)
    {
        case SP_UVEC2:
            glUniform2uiv(uniforms[uniName], count, value);
            break;
        case SP_UVEC3:
            glUniform3uiv(uniforms[uniName], count, value);
            break;
        case SP_UVEC4:
            glUniform4uiv(uniforms[uniName], count, value);
            break;
        default:
            errorStream << "Wrong type of uniform\n";
    }
}

void ShaderProgram::bindUniformVector(UTypes type, std::string uniName, const GLint *value, GLsizei count)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    switch (type)
    {
        case SP_IVEC2:
            glUniform2iv(uniforms[uniName], count, value);
            break;
        case SP_IVEC3:
            glUniform3iv(uniforms[uniName], count, value);
            break;
        case SP_IVEC4:
            glUniform4iv(uniforms[uniName], count, value);
            break;
        default:
            errorStream << "Wrong type of uniform\n";
    }
}

void ShaderProgram::bindUniformMatrix(UTypes type, std::string uniName, const GLfloat *value, GLboolean transpose,
                                      GLsizei count)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    switch (type)
    {
        case SP_MAT2:
            glUniformMatrix2fv(uniforms[uniName], count, transpose, value);
            break;
        case SP_MAT3:
            glUniformMatrix3fv(uniforms[uniName], count, transpose, value);
            break;
        case SP_MAT4:
            glUniformMatrix4fv(uniforms[uniName], count, transpose, value);
            break;
        default:
            errorStream << "Wrong type of uniform\n";
    }
}

void ShaderProgram::bindAttributeData(std::string attribName, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                      const GLvoid *pointer)
{
    if(attributes.count(attribName) == 0)
        assignAttribute(attribName);
    glEnableVertexAttribArray(attributes[attribName]);
    glVertexAttribPointer(attributes[attribName], size, type, normalized, stride, pointer);
}

void ShaderProgram::disableAttribute(std::string name)
{
    glDisableVertexAttribArray(attributes[name]);
}

bool ShaderProgram::link()
{
    program = glCreateProgram();
    if(vertex != 0)
        glAttachShader(program, vertex);
    if(fragment != 0)
        glAttachShader(program, fragment);
    if(geometry != 0)
        glAttachShader(program, geometry);
    if(compute != 0)
        glAttachShader(program, compute);
    glLinkProgram(program);
    GLint result;
    int logLength;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    glGetProgramInfoLog(program, logLength, NULL, &errorLog[0]);
    return result == GL_TRUE;
}

void ShaderProgram::use()
{
    glUseProgram(program);
}

void ShaderProgram::unuse()
{
    glUseProgram(0);
}

void ShaderProgram::printError()
{
    for(int i = 0; i < errorLog.size(); ++i)
        errorStream << errorLog[i];
    errorLog.clear();
}

GLuint ShaderProgram::getHandle()
{
    return program;
}

std::string ShaderProgram::loadFile(std::string filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        errorStream << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void ShaderProgram::assignUniform(std::string name)
{
    GLint uniform = glGetUniformLocation(program, name.c_str());
    uniforms[name] = uniform;
}

void ShaderProgram::assignAttribute(std::string name)
{
    GLint attribute = glGetAttribLocation(program, name.c_str());
    attributes[name] = attribute;
}