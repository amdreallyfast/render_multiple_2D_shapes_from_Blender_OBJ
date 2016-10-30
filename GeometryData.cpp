#include "GeometryData.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glload/include/glload/gl_4_4.h"

/*-----------------------------------------------------------------------------------------------
Description:
    Ensures that the structure starts object with initialized values.

    Note: OpenGL IDs can start at 0, but they are also unsigned, so they should not be set to
    -1.  Just initialize them to 0 and be sure to not use one without being initialized.
Parameters: None
Returns:    None
Creator:    John Cox (6-12-2016)
-----------------------------------------------------------------------------------------------*/
GeometryData::GeometryData() :
    _vaoId(0),
    _arrayBufferId(0),
    _drawStyle(0)
{
    // OpenGL IDs can start at 0, but they are also unsigned, so they should not be set to 
    // -1.  Just initialize them to 0 and be sure to not use one without being initialized.
}

/*-----------------------------------------------------------------------------------------------
Description:
    Generates a vertex buffer, index buffer, and vertex array object (contains vertex array
    attributes) for the provided geometry data.
Parameters:
    programId   Program binding is required for vertex attributes.
Returns:    None
Creator:    John Cox (6-12-2016)
-----------------------------------------------------------------------------------------------*/
void GeometryData::Init(unsigned int programId)
{
    // must bind program or else the vertex arrays will either blow up or refer to a 
    // non-existent program
    glUseProgram(programId);

    // vertex array buffer
    glGenBuffers(1, &_arrayBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _arrayBufferId);

    unsigned int vertBufferSizeBytes = _verts.size() * sizeof(_verts[0]);
    glBufferData(GL_ARRAY_BUFFER, vertBufferSizeBytes, _verts.data(), GL_STATIC_DRAW);

    // tell the GPU how the data will be organized per vertex
    glGenVertexArrays(1, &_vaoId);
    glBindVertexArray(_vaoId);

    unsigned int vertexArrayIndex = 0;
    unsigned int bufferStartOffset = 0;
    unsigned int bytesPerStep = sizeof(MyVertex);

    // position 
    GLenum itemType = GL_FLOAT;
    unsigned int numItems = sizeof(MyVertex::_position) / sizeof(float);
    glEnableVertexAttribArray(vertexArrayIndex);
    glVertexAttribPointer(vertexArrayIndex, numItems, itemType, GL_FALSE, bytesPerStep, (void *)bufferStartOffset);

    // normal
    itemType = GL_FLOAT;
    numItems = sizeof(MyVertex::_normal) / sizeof(float);
    bufferStartOffset += sizeof(MyVertex::_position);
    vertexArrayIndex++;
    glEnableVertexAttribArray(vertexArrayIndex);
    glVertexAttribPointer(vertexArrayIndex, numItems, itemType, GL_FALSE, bytesPerStep, (void *)bufferStartOffset);

    // must unbind array object BEFORE unbinding the buffer or else the array object will think 
    // that its vertex attribute pointers should refer the bound buffer ID (in this case, 0)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);
}
