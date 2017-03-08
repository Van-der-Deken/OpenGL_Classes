//
// Created by Y500 on 03.10.2016.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "ShaderProgram.h"

std::string SHADER_INFO::toString() const
{
    std::stringstream stringStream;
    stringStream << "Shader info:\n"
                 << "\tHandle:" << handle
                 << "\n\tPath to file with source:" << path.c_str()
                 << "\n\tShader type:";
    switch (type)
    {
        case GL_VERTEX_SHADER:
            stringStream << "Vertex shader\n";
            break;
        case GL_GEOMETRY_SHADER:
            stringStream << "Geometry shader\n";
            break;
        case GL_FRAGMENT_SHADER:
            stringStream << "Fragment shader\n";
            break;
        case GL_COMPUTE_SHADER:
            stringStream << "Compute shader\n";
            break;
        default:
            stringStream << "Undefined\n";
    }
    stringStream << "\tCompiled:" << compiled
                 << "\n\tAttached:" << attached << std::endl;
    return stringStream.str();
}

std::string PROGRAM_INFO::toString() const
{
    std::stringstream stringStream;
    stringStream << "Program info:"
                 << "\n   " << vertex.toString().c_str()
                 << "\n   " << geometry.toString().c_str()
                 << "\n   " << fragment.toString().c_str()
                 << "\n   " << compute.toString().c_str()
                 << "\n      Handle:" << handle
                 << "\n      Linked:" << linked
                 << "\n      Used:" << used << std::endl;
    return stringStream.str();
}

void ShaderProgram::setErrorStream(const std::ostream &inErrorStream)
{
    errorStream.rdbuf(inErrorStream.rdbuf());
}

bool ShaderProgram::loadShaderFromFile(const std::string &filePath, GLenum shaderType)
{
    std::string source = loadFile(filePath);
    if(source.empty())
        return false;
    switch (shaderType)
    {
        case GL_VERTEX_SHADER:
            program.vertex.path = filePath;
            break;
        case GL_GEOMETRY_SHADER:
            program.geometry.path = filePath;
            break;
        case GL_FRAGMENT_SHADER:
            program.fragment.path = filePath;
            break;
        case GL_COMPUTE_SHADER:
            program.compute.path = filePath;
            break;
    }
    return loadShaderFromString(source, shaderType);
}

bool ShaderProgram::loadShaderFromString(const std::string &source, GLenum shaderType)
{
    const char* cSource = source.c_str();
    GLint result = GL_FALSE;
    int logLength;

    SHADER_INFO shader;
    shader.handle = glCreateShader(shaderType);
    shader.type = shaderType;
    glShaderSource(shader.handle, 1, &cSource, NULL);
    glCompileShader(shader.handle);
    glGetShaderiv(shader.handle, GL_COMPILE_STATUS, &result);
    shader.compiled = result == GL_TRUE;
    glGetShaderiv(shader.handle, GL_INFO_LOG_LENGTH, &logLength);
    errorLog.resize(logLength);
    glGetShaderInfoLog(shader.handle, logLength, NULL, &errorLog[0]);

    switch (shaderType)
    {
        case GL_VERTEX_SHADER:
            shader.path = program.vertex.path;
            program.vertex = shader;
            break;
        case GL_FRAGMENT_SHADER:
            shader.path = program.fragment.path;
            program.fragment = shader;
            break;
        case GL_GEOMETRY_SHADER:
            shader.path = program.geometry.path;
            program.geometry = shader;
            break;
        case GL_COMPUTE_SHADER:
            shader.path = program.compute.path;
            program.compute = shader;
            break;
        default:
            errorStream << shaderType << ":unsupported type of shader\n";
            return false;
    }
    return result == GL_TRUE;
}

void ShaderProgram::bindUniform(const std::string &uniName, GLfloat v0)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    glUniform1f(uniforms[uniName], v0);
}

void ShaderProgram::bindUniformi(const std::string &uniName, int v0)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    glUniform1i(uniforms[uniName], v0);
}

void ShaderProgram::bindUniformui(const std::string &uniName, uint32_t v0)
{
    if(uniforms.count(uniName) == 0)
        assignUniform(uniName);
    glUniform1ui(uniforms[uniName], v0);
}

void ShaderProgram::bindUniformVector(UTypes type, const std::string &uniName, const GLfloat *value, GLsizei count)
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

void ShaderProgram::bindUniformVector(UTypes type, const std::string &uniName, const GLuint *value, GLsizei count)
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

void ShaderProgram::bindUniformVector(UTypes type, const std::string &uniName, const GLint *value, GLsizei count)
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

void ShaderProgram::bindUniformMatrix(UTypes type, const std::string &uniName, const GLfloat *value,
                                      GLboolean transpose, GLsizei count)
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

void ShaderProgram::bindAttributeData(const std::string &attribName, GLint size, GLenum type, GLboolean normalized,
                                      GLsizei stride, const GLvoid *pointer)
{
    if(attributes.count(attribName) == 0)
        assignAttribute(attribName);
    glEnableVertexAttribArray(attributes[attribName]);
    glVertexAttribPointer(attributes[attribName], size, type, normalized, stride, pointer);
}

void ShaderProgram::disableAttribute(const std::string &name)
{
    glDisableVertexAttribArray(attributes[name]);
}

bool ShaderProgram::link()
{
    program.handle = glCreateProgram();
    if(program.vertex.handle != 0)
    {
        glAttachShader(program.handle, program.vertex.handle);
        program.vertex.attached = true;
    }
    if(program.fragment.handle != 0)
    {
        glAttachShader(program.handle, program.fragment.handle);
        program.fragment.attached = true;
    }
    if(program.geometry.handle != 0)
    {
        glAttachShader(program.handle, program.geometry.handle);
        program.geometry.attached = true;
    }
    if(program.compute.handle != 0)
    {
        glAttachShader(program.handle, program.compute.handle);
    }
    glLinkProgram(program.handle);
    GLint result;
    int logLength;
    glGetProgramiv(program.handle, GL_LINK_STATUS, &result);
    program.linked = true;
    glGetProgramiv(program.handle, GL_INFO_LOG_LENGTH, &logLength);
    glGetProgramInfoLog(program.handle, logLength, NULL, &errorLog[0]);
    return result == GL_TRUE;
}


void ShaderProgram::use()
{
    glUseProgram(program.handle);
    program.used = true;
}

void ShaderProgram::unuse()
{
    glUseProgram(0);
    program.used = false;
}

void ShaderProgram::printError()
{
    for(int i = 0; i < errorLog.size(); ++i)
        errorStream << errorLog[i];
    errorLog.clear();
}

void ShaderProgram::deleteProgram()
{
    if(program.vertex.handle != 0)
        glDeleteShader(program.vertex.handle);
    if(program.geometry.handle != 0)
        glDeleteShader(program.geometry.handle);
    if(program.fragment.handle != 0)
        glDeleteShader(program.fragment.handle);
    if(program.compute.handle != 0)
        glDeleteShader(program.compute.handle);
    if(program.handle != 0)
        glDeleteProgram(program.handle);
    glUseProgram(0);
    PROGRAM_INFO empty;
    program = empty;
}

PROGRAM_INFO ShaderProgram::getProgramInfo() const
{
    return program;
}

std::string ShaderProgram::loadFile(const std::string &filePath)
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

void ShaderProgram::assignUniform(const std::string &name)
{
    GLint uniform = glGetUniformLocation(program.handle, name.c_str());
    uniforms[name] = uniform;
}

void ShaderProgram::assignAttribute(const std::string &name)
{
    GLint attribute = glGetAttribLocation(program.handle, name.c_str());
    attributes[name] = attribute;
}