#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
#define GlCall(x) GLClearError();\
                    x;\
                    ASSERT(GLLogCall(#x,__FILE__,__LINE__))
#else
#define GlCall(x) x
#endif

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, const int line) {
    while (const GLenum error = glGetError()) {
        std::cout << "[OpenGL Error]: " << error << " " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource {
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
};

static ShaderProgramSource parseShader(const std::string& filepath) {

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream shaderFile(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(shaderFile, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}

static unsigned int compileShader(unsigned int type, std::string& shaderSource) {
    unsigned int id = glCreateShader(type);
    const char* source = shaderSource.c_str();
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* msg = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, nullptr, msg);
        std::cout << "Error in " << (type==GL_VERTEX_SHADER?"vertex":"fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int createShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) {
    unsigned int id = glCreateProgram();
    unsigned int vertexId = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentId = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    glAttachShader(id, vertexId);
    glAttachShader(id, fragmentId);
    glLinkProgram(id);
    int status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::cout << "LINKING ERROR" << std::endl;
        char msg[512];
        glGetProgramInfoLog(id, 512, nullptr, msg);
        std::cout << msg << std::endl;
    }
    glValidateProgram(id);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    return id;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Error!" << std::endl;

    float vertices[] = {
        -0.5,-0.5,
        0.5,-0.5,
        0.5,0.5,
        -0.5,0.5,
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int indices[] = {
        0, 1 , 2,
        0, 2 , 3,
    };
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource shaderSource = parseShader("res/shaders/Basic.shader");
    unsigned int shaderProgram = createShaderProgram(shaderSource.vertexShaderSource,shaderSource.fragmentShaderSource);
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GlCall(glClear(GL_COLOR_BUFFER_BIT));

        GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        /* Swap front and back buffers */
        GlCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GlCall(glfwPollEvents());
    }
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
