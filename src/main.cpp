#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Engine.hpp"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

const std::string DEFAULT_FRAGMENT_SHADER = R"(
#version 330 core

in vec2 TexCoord;
in vec3 vertex_color;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(vertex_color, 1.0);
}
)";

const std::string DEFAULT_VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texureCoordinates;

out vec3 vertex_color;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertex_color = color;
    TexCoord = texureCoordinates;
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Engine::Logger::LogVerbose("GLAD initialized");

    float vertices[] = {
         -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
          0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
          0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    float differentColors[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int indicesCount = sizeof(indices) / sizeof(unsigned int);
    ASSERT(indicesCount == 6);

    {// local scope for GL destructors before the context is deleted
        Renderer renderer;
        renderer.SetClearColor(0.2f, 0.3f, 0.3f);

        Engine::Logger::LogCritical("Hello world");
        Engine::Logger::LogCritical("Hello world {0}", 123);

        ShaderProgram shader("DEFAULT_SHADER");
        shader.Create(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
        shader.Use();
        
        VertexArrayObject vao;
        
        VertexBuffer vb(vertices, sizeof(vertices));
        vb.DefineFloatAttribute(shader.GetAttribLocation("position"), 3);
        vb.DefineFloatAttribute(shader.GetAttribLocation("color"), 3);
        vb.DefineFloatAttribute(shader.GetAttribLocation("texureCoordinates"), 2);
        vao.AddBuffer(vb);

        //VertexBuffer colorBuffer(differentColors, sizeof(differentColors));
        //colorBuffer.DefineFloatAttribute(shader.GetAttribLocation("color"), 3);
        //vao.AddBuffer(colorBuffer);

        IndexBuffer ib(indices, indicesCount);

        Texture texture0("assets/flag.jpg");
        texture0.SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        texture0.SetMinMag(GL_LINEAR, GL_LINEAR);

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            renderer.Clear();
            renderer.Draw(vao, ib, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    Engine::Logger::LogVerbose("Close window");
    glfwTerminate();
    return 0;
}
