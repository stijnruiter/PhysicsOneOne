#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO: Make some kind of window properties structure
inline int Width(800);
inline int Height(600);

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Width = width;
    Height = height;
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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
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

    GLFWwindow* window = glfwCreateWindow(Width, Height, "PhysicsOneOne", NULL, NULL);
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
        // FORMAT X Y Z R G B TexX TexY
        // Vertices can only hold 1 texture coordinate, thus texures will be mirrored for 2 sides. 
         -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
          0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
          0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

         -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
          0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
          0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    float differentColors[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 
        0, 2, 3,

        1, 5, 6,
        1, 6, 2,

        0, 1, 5,
        0, 5, 4,

        0, 4, 7,
        0, 7, 3,

        3, 2, 6,
        3, 6, 7,

        4, 5, 6,
        4, 6, 7
    };

    unsigned int indicesCount = sizeof(indices) / sizeof(unsigned int);
    ASSERT(indicesCount == 36);

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

        VertexBuffer colorBuffer(differentColors, sizeof(differentColors));
        colorBuffer.DefineFloatAttribute(shader.GetAttribLocation("color"), 3);
        vao.AddBuffer(colorBuffer);

        IndexBuffer ib(indices, indicesCount);

        Texture texture0("assets/flag.jpg");
        texture0.SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        texture0.SetMinMag(GL_LINEAR, GL_LINEAR);

        GLCHECK(glEnable(GL_DEPTH_TEST));

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f), (float)Width / Height, 0.1f, 100.0f);

            shader.SetUniformMatrix4("model", model);
            shader.SetUniformMatrix4("view", view);
            shader.SetUniformMatrix4("projection", projection);

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
