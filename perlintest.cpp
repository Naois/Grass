#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "spacemaths.hpp"
#include "shader.hpp"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800,800,"cool",NULL,NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    float squareverts[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1
    };

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glCreateBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareverts), squareverts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    shader sqshad("shaders/perlintest/vert.vert", "shaders/perlintest/frag.frag");

    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime();

        sqshad.use();
        sqshad.setFloat(time, "time");
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}