//
// Created by Y500 on 26.03.2017.
//

#ifndef SDFRENDER_FONT_H
#define SDFRENDER_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <sstream>
#include "GLTexture.h"
#include "../glm/detail/type_vec.hpp"
#include "../glm/vec2.hpp"
#include "GLBuffer.h"
#include "ShaderProgram.h"
#include "../glm/detail/type_mat.hpp"
#include "../glm/detail/type_mat4x4.hpp"

/** Based on https://learnopengl.com/#!In-Practice/Text-Rendering **/

struct Character
{
    GLTexture texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;

    Character() : texture(), size(0), bearing(0), advance(0) {};
    Character(const Character &origin);
    Character& operator=(const Character& rValue);
};

class Font {
    public:
        Font(const std::ostream &inErrorStream = std::cout) : errorStream(inErrorStream.rdbuf()) {};
        Font(const FT_Library &library, const std::string &path, FT_Long faceIndex, FT_UInt height, FT_UInt width);
        ~Font();
        void setErrorStream(const std::ostream &inErrorStream);
        void setOrthoMatrix(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom);
        void setScale(GLfloat inScale);
        void setColor(const glm::vec3 &inColor);
        void loadProgram();
        void load(const FT_Library &library, const std::string &path, FT_Long faceIndex, FT_UInt height, FT_UInt width);
        Font& operator()(GLint x, GLint y);
        Font& render(const std::string &str);
        Font& render(const char *str);
        Font& render(char character);
        Font& render(GLfloat value);
        Font& render(GLdouble value);
        Font& render(GLint value);
        Font& render(GLuint value);
        Font& render(bool value);
    private:
        void glyphToTexture(GLchar character);

        FT_Face face;
        GLBuffer glyphCoord;
        ShaderProgram program;
        std::stringstream stringstream;
        glm::mat4 projection;
        glm::vec3 color;
        GLfloat scale;
        std::map<GLchar, Character> characters;
        std::ostream errorStream;
        glm::ivec2 startPosition;
};


#endif //SDFRENDER_FONT_H
