//
// Created by Y500 on 26.03.2017.
//

#include "Font.h"
#include "../glm/ext.hpp"

Character::Character(const Character &origin)
{
    texture = origin.texture;
    size = origin.size;
    bearing = origin.bearing;
    advance = origin.advance;
}

Character& Character::operator=(const Character &rValue)
{
    texture = rValue.texture;
    size = rValue.size;
    bearing = rValue.bearing;
    advance = rValue.advance;
}

Font::Font(const FT_Library &library, const std::string &path, FT_Long faceIndex, FT_UInt height, FT_UInt width) :
        errorStream(std::cout.rdbuf())
{
    if(FT_New_Face(library, path.c_str(), faceIndex, &face))
        errorStream << "Failed to load font\n";
    FT_Set_Pixel_Sizes(face, width, height);
    glyphCoord.setType(GL_ARRAY_BUFFER);
    glyphCoord.bind();
    glyphCoord.data(6 * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
}

Font::~Font()
{
    FT_Done_Face(face);
}

void Font::setErrorStream(const std::ostream &inErrorStream)
{
    errorStream.rdbuf(inErrorStream.rdbuf());
}

void Font::setOrthoMatrix(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom)
{
    projection = glm::ortho(left, right, bottom, top);
}

void Font::setScale(GLfloat inScale)
{
    scale = inScale;
}

void Font::setColor(const glm::vec3 &inColor)
{
    color = inColor;
}

void Font::loadProgram()
{
    const char vertexShader[] =
            "#version 330 core\n"
                    "layout (location = 0) in vec4 vertex;\n"
                    "out vec2 TexCoords;\n"
                    "uniform mat4 projection;\n"
                    "\n"
                    "void main(void) {\n"
                    "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
                    "    TexCoords = vertex.zw;\n"
                    "}\n";
    const char fragmentShader[] =
            "#version 330 core\n"
                    "in vec2 TexCoords;\n"
                    "out vec4 color;\n"
                    "uniform sampler2D text;\n"
                    "uniform vec3 textColor;\n"
                    "\n"
                    "void main(void) {\n"
                    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
                    "    color = vec4(textColor, 1.0) * sampled;\n"
                    "}\n";
    program.setErrorStream(errorStream);
    if(!program.loadShaderFromString(vertexShader, GL_VERTEX_SHADER))
        program.printError();
    if(!program.loadShaderFromString(fragmentShader, GL_FRAGMENT_SHADER))
        program.printError();
    if(!program.link())
        program.printError();
}

void Font::load(const FT_Library &library, const std::string &path, FT_Long faceIndex, FT_UInt height, FT_UInt width)
{
    FT_New_Face(library, path.c_str(), faceIndex, &face);
    FT_Set_Pixel_Sizes(face, width, height);
    glyphCoord.setType(GL_ARRAY_BUFFER);
    glyphCoord.bind();
    glyphCoord.data(6 * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
}

Font& Font::operator()(GLint x, GLint y)
{
    startPosition.x = x;
    startPosition.y = y;
    return *this;
}

Font& Font::render(const std::string &str)
{
    program.use();
    program.bindUniformVector(SP_VEC3, "textColor", glm::value_ptr(color));
    program.bindUniformMatrix(SP_MAT4, "projection", glm::value_ptr(projection), GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    std::string::const_iterator iter;
    for(iter = str.begin(); iter != str.end(); ++iter)
    {
        glyphToTexture(*iter);
        Character character = characters[*iter];
        GLfloat xPos = startPosition.x + character.bearing.x * scale;
        GLfloat yPos = startPosition.y + (character.size.y - character.bearing.y) * scale;
        GLfloat width = character.size.x * scale;
        GLfloat height = character.size.y * scale;

        GLfloat vertices[6][4] = {
                {xPos, yPos - height, 0.0f, 0.0f},
                {xPos, yPos, 0.0f, 1.0f},
                {xPos + width, yPos, 1.0f, 1.0f},

                {xPos, yPos - height, 0.0f, 0.0f},
                {xPos + width, yPos, 1.0f, 1.0f},
                {xPos + width, yPos - height, 1.0f, 0.0f}
        };
        character.texture.bind();
        glyphCoord.bind();
        glyphCoord.subData(0, sizeof(vertices), vertices);
        program.bindAttributeData("vertex", 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        startPosition.x += (character.advance >> 6) * scale;
    }
    glyphCoord.unbind();
    program.unuse();
    glBindTexture(GL_TEXTURE_2D, 0);
    return *this;
}

Font& Font::render(const char *str)
{
    std::string string(str);
    return render(string);
}

Font& Font::render(char character)
{
    stringstream.str("");
    stringstream << character;
    return render(stringstream.str());
}

Font& Font::render(GLfloat value)
{
    stringstream.str("");
    stringstream << value;
    return render(stringstream.str());
}

Font& Font::render(GLdouble value)
{
    stringstream.str("");
    stringstream << value;
    return render(stringstream.str());
}

Font& Font::render(GLint value)
{
    stringstream.str("");
    stringstream << value;
    return render(stringstream.str());
}

Font& Font::render(GLuint value)
{
    stringstream.str("");
    stringstream << value;
    return render(stringstream.str());
}

Font& Font::render(bool value)
{
    stringstream.str("");
    stringstream << std::boolalpha << value;
    return render(stringstream.str());
}

void Font::glyphToTexture(GLchar character)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if(characters.count(character) == 0)
    {
        if(FT_Load_Char(face, character, FT_LOAD_RENDER))
        {
            errorStream << "Failed to load " << character << " glyph\n";
            return;
        }
        Character glyph;
        glyph.texture.setType(GL_TEXTURE_2D);
        glyph.texture.bind();
        glyph.texture.specifyImage(0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
                                   GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glyph.texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glyph.texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glyph.texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glyph.texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glyph.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        glyph.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        glyph.advance = face->glyph->advance.x;
        characters[character] = glyph;
    }
}