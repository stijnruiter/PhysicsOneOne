#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline Camera camera(800, 600, glm::vec3(0, 0, 3), glm::vec3(0, 0, -1));

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
    window.SetCursorPosCallback([](double x, double y) 
    { 
        camera.UpdateMouse(x, y); // bind callback directly?
    }); 
    window.SetFrameBufferCallback([](int width, int height) 
    { 
        camera.UpdateFrameBuffer(width, height); 
        glViewport(0, 0, width, height); 
    });

    {// local scope for GL destructors before the context is deleted
        Renderer renderer;
        renderer.SetClearColor(0.2f, 0.3f, 0.3f);

        SceneBase* scene = new RotatingCube();

        GLCHECK(glEnable(GL_DEPTH_TEST));

        double deltaTime = 0.0;
        double lastFrame = 0.0;
        while (!window.ShouldClose())
        {
            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window, deltaTime);
            
            renderer.Clear();
            scene->Render(renderer, camera);

            window.SwapBuffers();
            glfwPollEvents();
        }
        delete scene;
    }
    Engine::Logger::LogVerbose("Close window");
    return 0;
}
