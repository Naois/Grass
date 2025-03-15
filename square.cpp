#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "spacemaths.hpp"
#include "shader.hpp"

int main()
{
    glfwInit();

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(mode->width,mode->height,"cool",glfwGetPrimaryMonitor(),NULL);
    glfwMakeContextCurrent(window);

    float width = mode->width;
    float height = mode->height;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = true;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    float squareverts[] = {
         0, 0,  0,
        40, 0,  0,
         0, 0, 40,
        40, 0, 40
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

    shader sqshad("shaders/groundplane.vert", "shaders/basic.frag");

    int verts[] = {
        0, 1, 2
    };

    GLuint grassvao;
    glCreateVertexArrays(1, &grassvao);
    glBindVertexArray(grassvao);
    GLuint grassvbo;
    glCreateBuffers(1, &grassvbo);
    glBindBuffer(GL_ARRAY_BUFFER, grassvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_INT, GL_FALSE, sizeof(int), (void*)0);

    shader grassshad("shaders/grass.vert", "shaders/basic.frag");

    double lasttime = glfwGetTime();

    float camx = 0, camy = 1, camz = 0;
    float theta = 0, phi = 0;
    bool mousecaptured = true;

    double oldmousex, oldmousey;

    float speed = 4;

    float fogred = 0.0, foggreen = 0.3, fogblue = 0.9;
    glClearColor(fogred,foggreen,fogblue,1);

    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double delta = time - lasttime;
        lasttime = time;

        double mousex, mousey;
        if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            glfwGetCursorPos(window, &mousex, &mousey);
        else
            {mousex = oldmousex; mousey = oldmousey;}
        theta -= 0.01*(mousey - oldmousey);
        phi += 0.01*(mousex - oldmousex);
        theta = theta < -M_PI_2? -M_PI_2 : theta;
        theta = theta > M_PI_2? M_PI_2 : theta;
        phi = fmod(phi, 2*M_PI);
        oldmousex = mousex;
        oldmousey = mousey;

        float forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS? delta * speed : 0;
        forward -= glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS? delta * speed : 0;

        vec4 motion(0,0,-forward,0);
        motion = yaw(phi) * pitch(theta) * motion;
        camx += motion.values[0];
        camy += motion.values[1];
        camz += motion.values[2];

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            mousecaptured = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(window, &oldmousex, &oldmousey);
        }

        mat4 pers = perspective(90, height / width, 0.01, 1000);
        mat4 camera = pitch(-theta) * yaw(-phi) * translate(-camx,-camy,-camz);

        sqshad.use();
        sqshad.setMat4(pers, "perspective");
        sqshad.setMat4(camera, "camera");
        sqshad.setVec4(fogred,foggreen,fogblue,1,"fogcolour");
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        grassshad.use();
        grassshad.setMat4(pers, "perspective");
        grassshad.setMat4(camera, "camera");
        grassshad.setMat4(yaw(phi) * pitch(theta), "billboard");
        grassshad.setFloat(time, "time");
        grassshad.setVec4(fogred,foggreen,fogblue,1,"fogcolour");

        glBindVertexArray(grassvao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 400*400);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}