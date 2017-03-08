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
            break;
        case GL_TEXTURE_2D:
            break;
        case GL_TEXTURE_3D:
            break;
        case GL_TEXTURE_1D_ARRAY:
            break;
        case GL_TEXTURE_2D_ARRAY:
            break;
        case GL_TEXTURE_RECTANGLE:
            break;
        case GL_TEXTURE_CUBE_MAP:
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            break;
        case GL_TEXTURE_BUFFER:
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            break;
        default:
    }
}