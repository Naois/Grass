#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;

void checkError()
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        cout << err << endl;
    }
}