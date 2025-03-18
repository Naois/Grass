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
    glfwWindowHint(GLFW_SAMPLES, 9);
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
        80, 0,  0,
         0, 0, 80,
        80, 0, 80
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

    shader sqshad("shaders/detailedgrass/groundplane.vert", "shaders/detailedgrass/ground.frag");

    int verts[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
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

    shader grassshad("shaders/detailedgrass/grass.vert", "shaders/detailedgrass/grass.frag");

    float skyverts[] = {
        -1, -1, 0.999999,
         1, -1, 0.999999,
        -1,  1, 0.999999,
         1,  1, 0.999999
    };

    GLuint skyvao;
    glCreateVertexArrays(1, &skyvao);
    glBindVertexArray(skyvao);
    GLuint skyvbo;
    glCreateBuffers(1, &skyvbo);
    glBindBuffer(GL_ARRAY_BUFFER, skyvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyverts), skyverts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);

    shader skyshad("shaders/sun/sun.vert", "shaders/sun/sun.frag");

    double lasttime = glfwGetTime();

    float camx = 40, camy = 1, camz = 40;
    float targettheta = 0, targetphi = 0;
    float theta = 0, phi = 0;
    bool mousecaptured = true;

    double oldmousex, oldmousey;
    glfwGetCursorPos(window, &oldmousex, &oldmousey);

    float speed = 4;

    float skyred = 0.0, skygreen = 0.4, skyblue = 1.0;

    glDisable(GL_CULL_FACE);

    bool lastspace = false;
    bool timepause = false;
    double time = 0;


    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double truetime = glfwGetTime();
        double delta = truetime - lasttime;
        lasttime = truetime;
        if(!timepause)
        {
            time += delta;
        }

        double mousex, mousey;
        if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            glfwGetCursorPos(window, &mousex, &mousey);
        else
            {mousex = oldmousex; mousey = oldmousey;}
        targettheta -= 0.01*(mousey - oldmousey);
        targetphi += 0.01*(mousex - oldmousex);
        targettheta = targettheta < -M_PI_2? -M_PI_2 : targettheta;
        targettheta = targettheta > M_PI_2? M_PI_2 : targettheta;
        float c = 0.95;
        theta = c*theta + (1-c)*targettheta;
        phi = c*phi + (1-c)*targetphi;
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

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if(!lastspace)
                timepause = !timepause;
            lastspace = true;
        }
        else
            lastspace = false;

        vec4 light = vec4(1,1,1,1);
        light = pitch(-time*0.1) * light;
        vec3 lightdir = light.xyz().normalize();


        sqshad.use();
        sqshad.setMat4(pers, "perspective");
        sqshad.setMat4(camera, "camera");
        sqshad.setVec3(lightdir, "lightdir");
        sqshad.setVec3(camx, camy, camz, "camerapos");
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        grassshad.use();
        grassshad.setMat4(pers, "perspective");
        grassshad.setMat4(camera, "camera");
        grassshad.setMat4(yaw(phi), "billboard");
        grassshad.setFloat(time, "time");
        grassshad.setVec3(lightdir, "lightdir");
        grassshad.setVec3(camx, camy, camz, "camerapos");

        glBindVertexArray(grassvao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 13, 1000*1000);

        mat4 screentoworlddir = yaw(phi) * pitch(theta) * screentodir(90, height / width);

        skyshad.use();
        skyshad.setMat4(screentoworlddir, "screentoworlddir");
        skyshad.setVec3(lightdir, "lightdir");
        skyshad.setVec3(skyred, skygreen, skyblue, "skycolour");

        glBindVertexArray(skyvao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}