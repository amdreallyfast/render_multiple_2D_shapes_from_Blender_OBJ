#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

#include "MyVertex.h"

/*-----------------------------------------------------------------------------------------------
Description:
    Stores all info necessary to draw a chunk of vertices and access the info later if
    neccessary.
Creator:    John Cox (10-23-2016)
-----------------------------------------------------------------------------------------------*/
struct GeometryData
{
    GeometryData();
    void Init(unsigned int programId);

    // save on the large header inclusion of OpenGL and write out these primitive types instead 
    // of using the OpenGL typedefs
    // Note: IDs are GLuint (unsigned int), draw style is GLenum (unsigned int).
    unsigned int _vaoId;
    unsigned int _arrayBufferId;
    unsigned int _drawStyle;  // GL_TRIANGLES, GL_LINES, etc.
    std::vector<MyVertex> _verts;
};