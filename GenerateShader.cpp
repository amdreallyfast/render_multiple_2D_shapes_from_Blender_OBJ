#include "GenerateShader.h"

#include "glload/include/glload/gl_4_4.h"

// for making program from shader collection
#include <string>
#include <fstream>
#include <sstream>

/*-----------------------------------------------------------------------------------------------
Description:
    Encapsulates the creation of an OpenGL GPU program, including the compilation and linking of
    shaders.  It tries to cover all the basics and the error reporting and is as self-contained
    as possible, only returning a program ID when it is finished.
Parameters: None
Returns:
    The OpenGL ID of the GPU program.
Exception:  Safe
Creator:    John Cox (2-13-2016)
-----------------------------------------------------------------------------------------------*/
unsigned int GenerateShaderProgram()
{
    // hard-coded ignoring possible errors like a boss

    // load up the vertex shader and compile it
    // Note: After retrieving the file's contents, dump the stringstream's contents into a 
    // single std::string.  Do this because, in order to provide the data for shader 
    // compilation, pointers are needed.  The std::string that the stringstream::str() function 
    // returns is a copy of the data, not a reference or pointer to it, so it will go bad as 
    // soon as the std::string object disappears.  To deal with it, copy the data into a 
    // temporary string.
    std::ifstream shaderFile("shader.vert");
    std::stringstream shaderData;
    shaderData << shaderFile.rdbuf();
    shaderFile.close();
    std::string tempFileContents = shaderData.str();
    GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertBytes[] = { tempFileContents.c_str() };
    const GLint vertStrLengths[] = { (int)tempFileContents.length() };
    glShaderSource(vertShaderId, 1, vertBytes, vertStrLengths);
    glCompileShader(vertShaderId);
    // alternately (if you are willing to include and link in glutil, boost, and glm), call 
    // glutil::CompileShader(GL_VERTEX_SHADER, shaderData.str());

    GLint isCompiled = 0;
    glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLchar errLog[128];
        GLsizei *logLen = 0;
        glGetShaderInfoLog(vertShaderId, 128, logLen, errLog);
        printf("vertex shader failed: '%s'\n", errLog);
        glDeleteShader(vertShaderId);
        return 0;
    }

    // load up the fragment shader and compiler it
    shaderFile.open("shader.frag");
    shaderData.str(std::string());      // because stringstream::clear() only clears error flags
    shaderData.clear();                 // clear any error flags that may have popped up
    shaderData << shaderFile.rdbuf();
    shaderFile.close();
    tempFileContents = shaderData.str();
    GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragBytes[] = { tempFileContents.c_str() };
    const GLint fragStrLengths[] = { (int)tempFileContents.length() };
    glShaderSource(fragShaderId, 1, fragBytes, fragStrLengths);
    glCompileShader(fragShaderId);

    glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLchar errLog[128];
        GLsizei *logLen = 0;
        glGetShaderInfoLog(fragShaderId, 128, logLen, errLog);
        printf("fragment shader failed: '%s'\n", errLog);
        glDeleteShader(vertShaderId);
        glDeleteShader(fragShaderId);
        return 0;
    }

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);

    // the program contains binary, linked versions of the shaders, so clean up the compile 
    // objects
    // Note: Shader objects need to be un-linked before they can be deleted.  This is ok because
    // the program safely contains the shaders in binary form.
    glDetachShader(programId, vertShaderId);
    glDetachShader(programId, fragShaderId);
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);

    // check if the program was built ok
    GLint isLinked = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        printf("program didn't compile\n");
        glDeleteProgram(programId);
        return 0;
    }

    // done here
    return programId;
}