#include "BlenderLoad.h"

#include <fstream>

#include <iostream>
using std::cout;
using std::endl;

#include "glload/include/glload/gl_4_4.h"


/*-----------------------------------------------------------------------------------------------
Description:
    Loads a set of vertices from a .obj file.  
    
    Note: Blender3D has the option, when exporting the scene, to use +Y as "up" or +Z as "up".  
    The .obj file itself says nothing about the matter.  This loading function will assume that 
    +Z is set as up since that is the way that OpenGL does it in 3D.

    Also Note: The .obj file does not specify any transform, so all the vertices and normals in 
    a .obj file are "as is".  Since this demo operates in window space (X and Y on the range 
    [-1,+1]) and on the depth range 0 to -1, the objects in the file must be constrained within 
    those bounds.

    
Parameters: 
    filePath    The path to the .obj file
    putDataHere An empty collection (will be cleared on function start) of GeometryData 
                structures by name. The vertices and normals will be extracted from the file and 
                the resulting vertex objects will be jammed into here.
Returns:    
    True if the function succeeded, otherwise false.
Exception:  Safe
Creator:    John Cox (2-13-2016)
-----------------------------------------------------------------------------------------------*/
bool BlenderLoad::LoadObj(const std::string &filePath, GEOMETRY_DATA_BY_NAME *putDataHere)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open())
    {
        cout << "Could not find the .obj file: " << filePath << endl;
        return false;
    }

    std::string line;
    std::getline(fileStream, line);
    size_t posOfSubstring = line.find("OBJ");
    if (posOfSubstring == -1)
    {
        cout << "File '" << filePath << "' is not an OBJ file" << endl;
    }

    std::vector<glm::vec3> vertPositions;
    //std::vector<glm::vec3> vertTextureCoord;    // not supported (yet)
    std::vector<glm::vec3> vertNormals;

    // parse it line by line
    // Note: Each line of data is preceded by a 1-char symbol.
    // Also Note: It is possible for a single object to have both an "f" and an "l", but if this 
    // has happened, this means that there is a mix of 2D and 3D vertices and something has gone 
    // wrong when manipulating the object in Blender.  For the sake of this demo, assume that if 
    // the object specifies "f" then it has faces and if it specifies "l" then it has lines.
    std::string lineHeaderObjectName("o ");
    std::string lineHeaderVertexPosition("v ");
    std::string lineHeaderVertexNormal("n ");
    std::string lineHeaderFace("f ");
    std::string lineHeaderLine("l ");

    while (std::getline(fileStream, line))
    {
        GeometryData *geometryData = 0;

        if (line.substr(0, lineHeaderObjectName.length()) == lineHeaderObjectName)
        {
            // new object
            
            // extract from the end of the head until the end of the line
            std::string objectName = line.substr(lineHeaderObjectName.length());
            
            // save this empty data
            putDataHere->insert(GEOMETRY_NAME_PAIR(objectName, GeometryData()));

            // now get a pointer to the GeometryData structure and work on that
            geometryData = &(putDataHere->find(objectName)->second);
            vertPositions.clear();
            //vertTextureCoord.clear();   // not supported (yet)
            vertNormals.clear();
        }
        else if (line.substr(0, lineHeaderVertexPosition.length()) == lineHeaderVertexPosition)
        {
            // as stated in the function description, expecting +Z to be up
            float x;
            float y;
            float z;
            const char *readBuffer = line.substr(lineHeaderVertexPosition.length()).c_str();
            sscanf(readBuffer, "%f %f %f", &x, &y, &z);
            vertPositions.push_back(glm::vec3(x, y, z));
        }
        else if (line.substr(0, lineHeaderVertexNormal.length()) == lineHeaderVertexNormal)
        {
            float x;
            float y;
            float z;
            const char *readBuffer = line.substr(lineHeaderVertexPosition.length()).c_str();
            sscanf(readBuffer, "%f %f %f", &x, &y, &z);
            vertNormals.push_back(glm::vec3(x, y, z));
        }
        else if (line.substr(0, lineHeaderLine.length()) == lineHeaderLine)
        {
            // data comes in lines
            // Note: I am assuming (for demo) that it only has lines.
            geometryData->_drawStyle = GL_LINES;

            // 2D lines in Blender do not have normals, but only positions
            unsigned short p1Index = 0;
            unsigned short p2Index = 0;

            // the 'h' indicates the reading of a short integer
            // Note: http://www.cprogramming.com/tutorial/printf-format-strings.html
            const char *readbuffer = line.substr(lineHeaderLine.length()).c_str();
            sscanf(readbuffer, "%hd %hd", &p1Index, &p2Index);

            // create the vertex objects and put them into the geometry data
            // Note: Indices for lines begin at 1 (not 0).
            // Also Note: +Z is up, and this is 2D, so I only need the X and Y out of the 
            // position, and thus I can jam the vec3 position into a vec2 and the constructor 
            // will cut off the Z.
            glm::vec2 p1(vertPositions[p1Index - 1]);
            glm::vec2 p2(vertPositions[p2Index - 1]);

            // remember that Blender3D does not specify normals for simple lines, so just stick 
            // in an empty normal.
            MyVertex v1(p1, glm::vec2());
            MyVertex v2(p2, glm::vec2());
            geometryData->_verts.push_back(v1);
            geometryData->_verts.push_back(v2);
        }
        else if (line.substr(0, lineHeaderFace.length()) == lineHeaderFace)
        {
            // data comes in quads (4 vertexes)
            // Note: I am assuming (for demo) that it only has quads, but quads have been 
            // deprecated for years, so I need to turn the quads into triangles.
            geometryData->_drawStyle = GL_TRIANGLES;

            // 'p' = position, 't' texture coordinate, 'n' = normal
            // Note: Texture coordinates not available yet, but handling them here anyway.
            unsigned short p1Index = 0, t1Index = 0, n1Index = 0;
            unsigned short p2Index = 0, t2Index = 0, n2Index = 0;
            unsigned short p3Index = 0, t3Index = 0, n3Index = 0;
            unsigned short p4Index = 0, t4Index = 0, n4Index = 0;

            // the 'h' indicates the reading of a short integer
            // Note: http://www.cprogramming.com/tutorial/printf-format-strings.html
            const char *readbuffer = line.substr(lineHeaderFace.length()).c_str();
            sscanf(readbuffer, "%hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd",
                &p1Index, &t1Index, &n1Index,
                &p2Index, &t2Index, &n2Index,
                &p3Index, &t3Index, &n3Index,
                &p4Index, &t4Index, &n4Index);

            // there is always position and normal, but not always texture
            // Note: The face indices begin at 1 (not 0), so if the index is 0, then there is no 
            // value.
            if (t1Index == 0 || t2Index == 0 || t3Index == 0 || t4Index == 0)
            {
                // no texture coordinates, so run the parse again
                sscanf(readbuffer, "%hd//%hd %hd//%hd %hd//%hd %hd//%hd",
                    &p1Index, &n1Index,
                    &p2Index, &n2Index,
                    &p3Index, &n3Index,
                    &p4Index, &n4Index);
            }

            // make triangles from the quad and put the data into the geometry structure
            // Note: For the sake of the this demo, which is in 2D, ignore the Z component.
            glm::vec2 p1(vertPositions[p1Index - 1]);
            glm::vec2 p2(vertPositions[p2Index - 1]);
            glm::vec2 p3(vertPositions[p3Index - 1]);
            glm::vec2 p4(vertPositions[p4Index - 1]);
            glm::vec2 n1(vertNormals[n1Index - 1]);
            glm::vec2 n2(vertNormals[n2Index - 1]);
            glm::vec2 n3(vertNormals[n3Index - 1]);
            glm::vec2 n4(vertNormals[n4Index - 1]);

            // these normals are meant for 3D and thus have a Z component, but right now that is 
            // being ignored, so the surface normals will not matter at render time
            MyVertex v1(p1, n1);
            MyVertex v2(p2, n2);
            MyVertex v3(p3, n3);
            MyVertex v4(p4, n4);
            
            // Note: Triangle 1 is v1, v2, v3.  
            geometryData->_verts.push_back(v1);
            geometryData->_verts.push_back(v2);
            geometryData->_verts.push_back(v3);
            
            // Note: Triangle 2 is v3, v4, v1.
            geometryData->_verts.push_back(v3);
            geometryData->_verts.push_back(v4);
            geometryData->_verts.push_back(v1);

            // doing a triangle, so 
        }
        else
        {
            // ??unknown header??
        }
    }
}
