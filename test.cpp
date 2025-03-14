#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "spacemaths.hpp"
#include "shader.hpp"

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800,800,"cool",NULL,NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    float squareverts[] = {
        -1, -1, 0,
         1, -1, 0,
        -1,  1, 0,
         1,  1, 0
    };

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glCreateBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareverts), squareverts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    shader grass("shaders/grass.vert", "shaders/basic.frag");
    shader square("shaders/basic.vert", "shaders/basic.frag");

    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float x=5, y=0, z=0;
        float theta=0, phi=0;
        mat4 pers = perspective() * yaw(phi) * pitch(theta) * translate(x,y,z);

        // grass.use();
        // grass.setMat4(pers, "view");

        square.use();
        square.setMat4(pers, "view");
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 10000);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}