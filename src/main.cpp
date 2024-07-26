#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1));

void processInput(Window& window, double deltaTime)
{
    if (window.IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        window.Close();
        return;
    }
    camera.UpdateInput(window, (float)deltaTime);
}

int main()
{
    Engine::Logger::LogInfo("--------------------------------");
    Engine::Logger::LogInfo("-------CREATE NEW SESSION-------");
    Engine::Logger::LogInfo("--------------------------------");

    Window window(800, 600, "Physics 101");
    window.SetMouseCursor(false);
    window.SetCursorPosCallback([](double x, double y) { camera.UpdateMouse(x, y); }); // bind callback directly?
    window.SetFrameBufferCallback([](int width, int height) { glViewport(0, 0, width, height); });

    
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

        double deltaTime = 0.0;
        double lastFrame = 0.0;
        while (!window.ShouldClose())
        {
            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window, deltaTime);
            glm::mat4 model(1);
            model = glm::rotate(model, (float)currentFrame, glm::vec3(0, 1, 0));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), window.GetAspectRatio(), 0.1f, 100.0f);

            shader.SetUniformMatrix4("model", model);
            shader.SetUniformMatrix4("view", camera.GetView());
            shader.SetUniformMatrix4("projection", projection);

            renderer.Clear();
            renderer.Draw(vao, ib, shader);

            window.SwapBuffers();
            glfwPollEvents();
        }
    }
    Engine::Logger::LogVerbose("Close window");
    return 0;
}
