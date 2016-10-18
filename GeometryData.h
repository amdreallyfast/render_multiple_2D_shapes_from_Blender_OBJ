#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

struct MyVertex
{
    glm::vec2 _position;
    glm::vec3 _color;
};

/*-----------------------------------------------------------------------------------------------
Description:
    Stores all info necessary to draw a chunk of vertices and access the info later if
    neccessary.
Creator:    John Cox (6-12-2016)
-----------------------------------------------------------------------------------------------*/
struct GeometryData
{
    GeometryData() :
        _vaoId(0),
        _arrayBufferId(0),
        _elementBufferId(0)
    {
        // OpenGL IDs can start at 0, but they are also unsigned, so they should not be set to 
        // -1.  Just initialize them to 0 and be sure to not use one without being initialized.
    }

    // save on the large header inclusion of OpenGL and write out these primitive types instead 
    // of using the OpenGL typedefs
    // Note: IDs are GLuint (unsigned int), draw style is GLenum (unsigned int), GLushort is 
    // unsigned short.
    unsigned int _vaoId;
    unsigned int _arrayBufferId;
    unsigned int _elementBufferId;
    unsigned int _drawStyle;  // GL_TRIANGLES, GL_LINES, etc.
    std::vector<MyVertex> _verts;
    std::vector<unsigned short> _indices;
};