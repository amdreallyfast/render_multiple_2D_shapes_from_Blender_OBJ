#pragma once

#include <string>
#include <map>
#include <vector>
#include "GeometryData.h"


/*-----------------------------------------------------------------------------------------------
Description:
    Loads a set of vertexes for each object in an exported Blender3D file into a single 
    collection.  It is the responsibility of the user to then take this data and store it in 
    buffer objects as they see fit.

    Note: When defining faces and lines, almost every single face-normal pair is unique.  This 
    means that there is little benefit to using element arrays, which were created to allow 
    re-use of existing vertices.  Instead, I will simply use the vertex arrays.
Creator:    John Cox (10-23-2016)
-----------------------------------------------------------------------------------------------*/
class BlenderLoad
{
public:
    // Note:
    // - std::string    The name of the object given by the file.
    // - GeometryData   The draw style (lines or triangles) and the vertex data.
    typedef std::map<std::string, GeometryData> GEOMETRY_DATA_BY_NAME;

    static bool LoadObj(const std::string &filePath, GEOMETRY_DATA_BY_NAME *putDataHere);

private:
    // for use in pushing a value onto the map
    typedef std::pair<std::string, GeometryData> GEOMETRY_NAME_PAIR;
};