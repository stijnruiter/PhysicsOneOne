#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const std::string DEFAULT_FRAGMENT_SHADER = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}
)";

const std::string DEFAULT_VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0);
}
)";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "PhysicsOneOne", NULL, NULL);
    if (window == NULL)
    {
        Engine::Logger::LogError("NOWINDOW");
        glfwTerminate();
        return -1;
    }
    Engine::Logger::LogVerbose("GLFW initialized");

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Engine::Logger::LogError("NOGLAD");
        return -1;
    }

    Engine::Logger::LogVerbose("GLAD initialized");

    GLCHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    float vertices[] = {
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vSource = DEFAULT_VERTEX_SHADER.c_str();
    GLCHECK(glShaderSource(vertexShader, 1, &vSource, NULL));
    GLCHECK(glCompileShader(vertexShader));

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = DEFAULT_FRAGMENT_SHADER.c_str();
    GLCHECK(glShaderSource(fragmentShader, 1, &fSource, NULL));
    GLCHECK(glCompileShader(fragmentShader));

    GLint status;
    GLCHECK(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint length;
        GLCHECK(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length));
        GLchar* infoLog = new GLchar[length];
        GLCHECK(glGetShaderInfoLog(vertexShader, length, NULL, infoLog));

        Engine::Logger::LogError("ERROR::SHADER::COMPILATION_FAILED");
        Engine::Logger::LogError(infoLog);

        DEBUGBREAK();

        delete[] infoLog;
        GLCHECK(glDeleteShader(vertexShader));

        return -1;
    }

    GLCHECK(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint length;
        GLCHECK(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length));
        GLchar* infoLog = new GLchar[length];
        GLCHECK(glGetShaderInfoLog(fragmentShader, length, NULL, infoLog));

        Engine::Logger::LogError("ERROR::SHADER::COMPILATION_FAILED");
        Engine::Logger::LogError(infoLog);

        DEBUGBREAK();

        delete[] infoLog;
        GLCHECK(glDeleteShader(fragmentShader));

        return -1;
    }

    GLuint program = glCreateProgram();
    GLCHECK(glAttachShader(program, vertexShader));
    GLCHECK(glAttachShader(program, fragmentShader));
    GLCHECK(glLinkProgram(program));

    GLCHECK(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint length;
        GLCHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        GLchar* infoLog = new GLchar[length];
        GLCHECK(glGetProgramInfoLog(program, length, NULL, infoLog));

        Engine::Logger::LogError("ERROR::SHADER::LINK_ERROR");
        Engine::Logger::LogError(infoLog);

        DEBUGBREAK();
        delete[] infoLog;

        GLCHECK(glDeleteProgram(program));
        program = 0;
        return -1;
    }

    GLCHECK(glDeleteShader(vertexShader));
    GLCHECK(glDeleteShader(fragmentShader));

    GLCHECK(glUseProgram(program));


    unsigned int vertexArrayObject;
    GLCHECK(glGenVertexArrays(1, &vertexArrayObject));
    GLCHECK(glBindVertexArray(vertexArrayObject));

    unsigned int vertexBuffer;
    GLCHECK(glGenBuffers(1, &vertexBuffer));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    int location = glGetAttribLocation(program, "position");
    if (location < 0)
    {
        Engine::Logger::LogError("Unable to retrieve attribute location for 'position'.");
        DEBUGBREAK();
        return -1;
    }

    GLCHECK(glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCHECK(glEnableVertexAttribArray(location));

    unsigned int elementBuffer;
    GLCHECK(glGenBuffers(1, &elementBuffer));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        GLCHECK(glClear(GL_COLOR_BUFFER_BIT));

        GLCHECK(glUseProgram(program));
        GLCHECK(glBindVertexArray(vertexArrayObject));
        GLCHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // TODO: There are still some early returns
    GLCHECK(glDeleteBuffers(1, &vertexBuffer));
    GLCHECK(glDeleteBuffers(1, &elementBuffer));
    GLCHECK(glDeleteVertexArrays(1, &vertexArrayObject));
    GLCHECK(glDeleteProgram(program));

    Engine::Logger::LogVerbose("Close window");
    glfwTerminate();
    return 0;
}
