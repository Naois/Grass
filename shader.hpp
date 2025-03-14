
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include "ErrorCheck.hpp"
#include "spacemaths.hpp"

using namespace std;

class shader
{
public:
    GLuint programID;
    shader() = default;
    shader(string vertloc, string fragloc)
    {
        ifstream vertfile(vertloc);
        if(!vertfile.is_open())
        {
            cout << "Failed to find file \"" << vertloc << "\"" << endl;
            exit(1);
        }
        string text;
        string line;
        while(getline(vertfile, line))
        {
            text += line;
            text += "\n";
        }
        GLuint vertshad = glCreateShader(GL_VERTEX_SHADER);
        {
            const char* source = text.c_str();
            const int length = text.size();
            glShaderSource(vertshad, 1, &source, &length);
            glCompileShader(vertshad);
        }
        text.clear();
        vertfile.close();
        char buffer[1024];
        GLint comp;
        GLsizei len;
        glGetShaderiv(vertshad, GL_COMPILE_STATUS, &comp);
        if(comp == GL_FALSE)
        {
            glGetShaderInfoLog(vertshad, 1024, &len, buffer);
            cout << "Vertex shader \"" << vertloc << "\" failed to compile: \n" << buffer << endl;
        }

        ifstream fragfile(fragloc);
        if(!fragfile.is_open())
        {
            cout << "Failed to find file \"" << fragloc << "\"" << endl;
            exit(1);
        }
        while(getline(fragfile, line))
        {
            text += line;
            text += "\n";
        }
        GLuint fragshad = glCreateShader(GL_FRAGMENT_SHADER);
        {
            const char* source = text.c_str();
            const int length = text.size();
            glShaderSource(fragshad, 1, &source, &length);
            glCompileShader(fragshad);
        }
        fragfile.close();
        glGetShaderiv(fragshad, GL_COMPILE_STATUS, &comp);
        if(comp == GL_FALSE)
        {
            glGetShaderInfoLog(fragshad, 1024, &len, buffer);
            cout << "Fragment shader \"" << fragloc << "\" failed to compile: \n" << buffer << endl;
        }
        
        programID = glCreateProgram();
        glAttachShader(programID, vertshad);
        glAttachShader(programID, fragshad);
        glLinkProgram(programID);
        glGetProgramiv(programID, GL_LINK_STATUS, &comp);
        if(comp == GL_FALSE)
        {
            glGetProgramInfoLog(programID, 1024, &len, buffer);
            cout << "Shader failed to link: \n" << buffer << endl;
        }
        glDeleteShader(vertshad);
        glDeleteShader(fragshad);
        
    }

    void use()
    {
        glUseProgram(programID);
    }

    // Make sure to use() ^^^ before you call these uniform functions
    void setFloat(float val, const char* name)
    {
        glUniform1f(glGetUniformLocation(programID, name), val);
    }

    void setVec2(float x, float y, const char* name)
    {
        glUniform2f(glGetUniformLocation(programID, name), x, y);
    }

    void setVec4(float x, float y, float z, float w, const char* name)
    {
        glUniform4f(glGetUniformLocation(programID, name), x, y, z, w);
    }

    void setMat4(mat4 m, const char* name)
    {
        glUniformMatrix4fv(glGetUniformLocation(programID,name),1,GL_TRUE,m.values);
    }

    ~shader()
    {
        glDeleteProgram(programID);
        programID = 0;
    }
};