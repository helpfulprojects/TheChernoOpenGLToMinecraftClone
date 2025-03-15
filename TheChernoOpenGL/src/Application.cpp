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
    GlCall(glShaderSource(id, 1, &source, nullptr));
    GlCall(glCompileShader(id));
    int status;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        int length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* msg = (char*)alloca(length*sizeof(char));
        GlCall(glGetShaderInfoLog(id, length, nullptr, msg));
        std::cout << "Error in " << (type==GL_VERTEX_SHADER?"vertex":"fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;
        GlCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

static unsigned int createShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) {
    unsigned int id = glCreateProgram();
    unsigned int vertexId = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentId = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GlCall(glAttachShader(id, vertexId));
    GlCall(glAttachShader(id, fragmentId));
    GlCall(glLinkProgram(id));
    int status;
    GlCall(glGetProgramiv(id, GL_LINK_STATUS, &status));
    if (status == GL_FALSE) {
        std::cout << "LINKING ERROR" << std::endl;
        char msg[512];
        glGetProgramInfoLog(id, 512, nullptr, msg);
        std::cout << msg << std::endl;
    }
    GlCall(glValidateProgram(id));
    GlCall(glDeleteShader(vertexId));
    GlCall(glDeleteShader(fragmentId));
    return id;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    float vertices[] = {
        -0.5,-0.5,
        0.5,-0.5,
        0.5,0.5,
        -0.5,0.5,
    };
    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    unsigned int vbo;
    GlCall(glGenBuffers(1, &vbo));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GlCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW));

    GlCall(glEnableVertexAttribArray(0));
    GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int indices[] = {
        0, 1 , 2,
        0, 2 , 3,
    };
    unsigned int ebo;
    GlCall(glGenBuffers(1, &ebo));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    glBindVertexArray(0);
    ShaderProgramSource shaderSource = parseShader("res/shaders/Basic.shader");
    unsigned int shaderProgram = createShaderProgram(shaderSource.vertexShaderSource,shaderSource.fragmentShaderSource);
    GlCall(glUseProgram(shaderProgram));
    GlCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    ASSERT(location != -1);
    GlCall(glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f));

    GlCall(glUseProgram(0));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GlCall(glClear(GL_COLOR_BUFFER_BIT));

		GlCall(glUseProgram(shaderProgram));
		GlCall(glUniform4f(location, r, 0.0f, 0.0f, 1.0f));
		GlCall(glBindVertexArray(vao));
        GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r < 0.0f)
            increment = 0.05f;
        else if (r > 1.0f)
            increment = -0.05f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    GlCall(glDeleteProgram(shaderProgram));
    glfwTerminate();
    return 0;
}
