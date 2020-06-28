#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <mingw.thread.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H
#include "Reversi.h"
#include "PlayerAgent.h"
#include "ComputerAgent.h"
#include "Shader.h"

int display_init();
void thread_function();

std::vector<char, std::allocator<char>> state;
unsigned int gridVAO, gridVBO, gridEBO, textVAO, textVBO, VAO, VBO, EBO, shaderProgram, uniformLocationColor;
std::vector<unsigned int> circleVAOS, circleVBOS;
GLFWwindow* window;
bool mouseIn = false;
int windowWidth, windowHeight, frameBufferWidth, frameBufferHeight;
PlayerAgent agent1;
ComputerAgent agent2;
Reversi reversi(agent1, agent2);
Shader* textShader;
std::thread t(&thread_function);

int main()
{
//    t.detach();
    return display_init();
}

void thread_function(){
    reversi.init();
    reversi.play();
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
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

void RenderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color, bool alignCenter = false)
{
    // activate corresponding render state
    shader->use();
    glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // iterate through all characters
    std::string::const_iterator c;
    if(alignCenter){
        float totalWidth = 0;
        for (c = text.begin(); c != text.end(); c++){
            Character ch = Characters[*c];
            totalWidth += (ch.Advance >> 6) * scale;
        }
        x -= totalWidth/2;
//        y -= Characters[*text.begin()].Size.y/2;
    }

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        // update VBO for each character
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void setup_draw(){
    std::string vertexShaderSrc = ReadFile("src/shaders/vertex_basic.glsl");
    std::string fragmentShaderSrc = ReadFile("src/shaders/fragment_basic.glsl");
    shaderProgram = CreateShader(vertexShaderSrc, fragmentShaderSrc);
    uniformLocationColor = glGetUniformLocation(shaderProgram, "u_Color");

    textShader = new Shader("src/shaders/vertex_text.glsl", "src/shaders/fragment_text.glsl");
    textShader->use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(frameBufferWidth), 0.0f, static_cast<float>(frameBufferWidth));
    glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, "resources/font/Roboto-Regular.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 128);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // circles
    for (int i = 0; i < 64; ++i) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        circleVAOS.push_back(VAO);
        circleVBOS.push_back(VBO);
        std::vector<float> tmp = CreateCircleArray(0.08, -0.7 + i%8*0.2, 0.7 - i/8*0.2, 100);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(float), &tmp.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw(){
    /* Render here */
    glClearColor(0.8, 0.8, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    std::pair<int, int> score = reversi.getBoard().get_score();
    std::string standings = "Black " + std::to_string(score.first) + " - White " + std::to_string(score.second);
    RenderText(textShader, standings, ((float)frameBufferWidth)/2, (float)(frameBufferHeight*0.92), ((float)frameBufferHeight)/2000, glm::vec3(0.2f, 0.2f, 0.2f), true);

    glUseProgram(shaderProgram);
    // draw grid
    glBindVertexArray(gridVAO);
    glUniform4f(uniformLocationColor, 0, 0, 0, 1);
    glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // draw circles
    for (int i = 0; i < 64; ++i) {
        char piece = reversi.getBoard().map[i];
        if(piece == 'O'){
            glBindVertexArray(circleVAOS[i]);
            glUniform4f(uniformLocationColor, 0.1, 0.1, 0.1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 100);
            glBindVertexArray(0);
        }else if(piece == 'X'){
            glBindVertexArray(circleVAOS[i]);
            glUniform4f(uniformLocationColor, 1, 1, 1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 100);
            glUniform4f(uniformLocationColor, 0.1, 0.1, 0.1, 1);
            glDrawArrays(GL_LINE_LOOP, 0, 100);
            glBindVertexArray(0);
        }else if(piece == '-'){
        }
    }

    if(reversi.gameEnd()){

        if(score.first > score.second){
            RenderText(textShader, "Black Win", ((float)frameBufferWidth)/2, ((float)frameBufferHeight)/2, ((float)frameBufferHeight)/2000, glm::vec3(0.2f, 0.2f, 0.2f), true);
        }else if(score.first < score.second){
            RenderText(textShader, "White Win", ((float)frameBufferWidth)/2, ((float)frameBufferHeight)/2, ((float)frameBufferHeight)/2000, glm::vec3(0.2f, 0.2f, 0.2f), true);
        }else{
            RenderText(textShader, "Draw", ((float)frameBufferWidth)/2, ((float)frameBufferHeight)/2, ((float)frameBufferHeight)/2000, glm::vec3(0.2f, 0.2f, 0.2f), true);
        }
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void window_size_callback(GLFWwindow* window, int width, int height){
    windowWidth = width;
    windowHeight = height;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    frameBufferWidth = width;
    frameBufferHeight = height;
    textShader->use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(frameBufferWidth), 0.0f, static_cast<float>(frameBufferWidth));
    glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
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
        double xpos, ypos, glxpos, glypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glxpos = xpos/(windowWidth-1)*2-1;
        glypos = -ypos/(windowHeight-1)*2+1;
        printf("%f, %f\n", xpos, ypos);
        printf("%f, %f\n", glxpos, glypos);
        if(typeid(agent1) == typeid(PlayerAgent) && dynamic_cast<PlayerAgent&>(agent1).readClicks){
            std::cout << "player1" << std::endl;
            dynamic_cast<PlayerAgent&>(agent1).clicks.emplace(glxpos, glypos);
        }
        if(typeid(agent2) == typeid(PlayerAgent) && dynamic_cast<PlayerAgent&>(agent2).readClicks){
            std::cout << "player2" << std::endl;
            dynamic_cast<PlayerAgent&>(agent2).clicks.emplace(glxpos, glypos);
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
//        t.join();
        t.join();
        t = std::thread(&thread_function);
    }
}

int display_init(){
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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