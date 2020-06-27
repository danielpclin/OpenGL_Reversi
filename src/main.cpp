#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Reversi.h"

unsigned int gridVAO, gridVBO, gridEBO, VAO, VBO, EBO, shaderProgram, uniformLocationColor;
std::vector<unsigned int> circleVAOS, circleVBOS;
GLFWwindow* window;
bool mouseIn = false;
int windowWidth, windowHeight, frameBufferWidth, frameBufferHeight;

int init();

int main()
{
//    PlayerAgent player1;
//    PlayerAgent player2;
//    Reversi reversi(player1, player2);
//    reversi.start();

    init();

    return 0;
}

unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *)malloc(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        free(message);
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

std::string ReadFile(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::stringstream ss;

    std::string line;
    while (getline(stream, line))
    {
        ss << line << '\n';
    }
    stream.close();
    return ss.str();
}

std::vector<float> CreateCircleArray(float radius, float x, float y, int fragments)
{
    const float PI = 3.1415926f;

    std::vector<float> result;

    float increment = 2.0f * PI / fragments;

    float currAngle = 0.0f;
    for (int i = 0; i < fragments; i++)
    {

        result.push_back(radius * std::cos(currAngle) + x);
        result.push_back(radius * std::sin(currAngle) + y);
        currAngle += increment;
    }

    return result;
}

void setup_draw(){
    std::string vertexShaderSrc = ReadFile("src/shaders/vertex_basic.glsl");
    std::string fragmentShaderSrc = ReadFile("src/shaders/fragment_basic.glsl");
    shaderProgram = CreateShader(vertexShaderSrc, fragmentShaderSrc);
    uniformLocationColor = glGetUniformLocation(shaderProgram, "u_Color");

    // glGenBuffers(1, &EBO);
    // grid
    std::vector<float> gridVertices;
    for (int i = 0; i < 8; ++i) {
        float x = -0.8 + i*0.2, y = 0.8;
        gridVertices.push_back(x);
        gridVertices.push_back(y);
    }
    for (int i = 0; i < 8; ++i) {
        float x = 0.8, y = 0.8 - i*0.2;
        gridVertices.push_back(x);
        gridVertices.push_back(y);
    }
    for (int i = 0; i < 8; ++i) {
        float x = 0.8 - i*0.2, y = -0.8;
        gridVertices.push_back(x);
        gridVertices.push_back(y);
    }
    for (int i = 0; i < 8; ++i) {
        float x = -0.8, y = -0.8 + i*0.2;
        gridVertices.push_back(x);
        gridVertices.push_back(y);
    }
    std::vector<int> gridIndices;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glGenBuffers(1, &gridEBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), &gridVertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    for (int i = 0; i <= 8; ++i) {
        gridIndices.push_back(i);
        gridIndices.push_back(24-i);
    }
    for (int i = 32; i >= 24; --i) {
        gridIndices.push_back(i%32);
        gridIndices.push_back(40-i);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridIndices.size() * sizeof(float), &gridIndices.front(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // circles
    for (int i = 0; i < 64; ++i) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        circleVAOS.push_back(VAO);
        circleVBOS.push_back(VBO);
        std::vector<float> tmp = CreateCircleArray(0.08, -0.7 + i%7*0.2, 0.7 - i/7*0.2, 40);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(float), &tmp.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    glBindVertexArray(0);
}

void draw(){
    /* Render here */
    glClearColor(0.9, 0.9, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);

    // draw grid
    glBindVertexArray(gridVAO);
    glUniform4f(uniformLocationColor, 0, 0, 0, 1);
    glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindVertexArray(circleVAOS[1]);
    glUniform4f(uniformLocationColor, 0.1, 0.1, 0.1, 1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 40);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void window_size_callback(GLFWwindow* window, int width, int height){
    windowWidth = width;
    windowHeight = height;
//    printf("window: %d, %d\n", width, height);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
//    printf("framebuffer: %d, %d\n", width, height);
    frameBufferWidth = width;
    frameBufferHeight = height;
    glViewport(0, 0, width, height);
    draw();
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        // The cursor entered the content area of the window
        mouseIn = true;
    }
    else
    {
        // The cursor left the content area of the window
        mouseIn = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//    printf("%f, %f", xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
//        printf("%f, %f\n", xpos, ypos);
//        printf("%f, %f\n", xpos/(windowWidth-1)*2-1, -ypos/(windowHeight-1)*2+1);

    }
}


int init(){
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "黑白棋", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowAspectRatio(window, 1, 1);
    if(glewInit() != GLEW_OK){
        std::cout <<"GLEW Error" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

    setup_draw();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        draw();
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void render() {

}
