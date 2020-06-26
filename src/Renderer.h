#ifndef OPENGL_PRACTICE_RENDERER_H
#define OPENGL_PRACTICE_RENDERER_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class Renderer {
    std::vector<int> board;
    GLFWwindow* window;
    unsigned int VAO, VBO, EBO, shaderProgram;
    void draw();

public:

    void render();

    int init();

    static std::vector<float> CreateCircleArray(float radius, float x, float y, int fragments);

};


#endif //OPENGL_PRACTICE_RENDERER_H
