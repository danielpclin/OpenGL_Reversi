
#include "Renderer.h"

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
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

static std::string ReadFile(const std::string& filePath)
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

std::vector<float> Renderer::CreateCircleArray(float radius, float x, float y, int fragments)
{
    const float PI = 3.1415926f;

    std::vector<float> result;

    float increment = 2.0f * PI / fragments;

    float currAngle = 0.0f;
    for (int i = 0; i < fragments; i++)
    {
        std::cout << "angle" << currAngle << std::endl;
        std::cout << "x" << radius * std::cos(currAngle) << std::endl;
        std::cout << "y" << radius * std::sin(currAngle) << std::endl;

        result.push_back(radius * std::cos(currAngle) + x);
        result.push_back(radius * std::sin(currAngle) + y);
        currAngle += increment;
    }

    return result;
}

void Renderer::draw(){
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLE_FAN, 0, 20);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int Renderer::init(){

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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);

    std::vector<float> a = CreateCircleArray(0.1, 0, 0, 20);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, a.size() * sizeof(float), &a.front(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);

    std::string vertexShaderSrc = ReadFile("src/shaders/vertex_basic.glsl");
    std::string fragmentShaderSrc = ReadFile("src/shaders/fragment_basic.glsl");
    shaderProgram = CreateShader(vertexShaderSrc, fragmentShaderSrc);

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

void Renderer::render() {

}
