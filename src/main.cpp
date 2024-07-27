#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

int main()
{
    Engine::Logger::LogInfo("--------------------------------");
    Engine::Logger::LogInfo("-------CREATE NEW SESSION-------");
    Engine::Logger::LogInfo("--------------------------------");

    Window window(800, 600, "Physics 101");
    Camera camera(window.GetWidth(), window.GetHeight(), glm::vec3(0, 0, 3), glm::vec3(0, 0, -1));
    window.SetMouseCursor(false);
    window.SetCallbackOnMouseMove([&camera](const MouseMoveEvent& eventArg) 
    {
        camera.UpdateMouse(eventArg.X, eventArg.Y);
    });
    window.SetCallbackOnFrameBufferResize([&camera](const FrameBufferResizeEvent& eventArgs) 
    {
        camera.UpdateFrameBuffer(eventArgs.Width, eventArgs.Height);
    });


    {// local scope for GL destructors before the context is deleted
        Renderer renderer;
        renderer.SetClearColor(0.2f, 0.3f, 0.3f);

        std::unique_ptr<SceneBase> scene = std::make_unique<RotatingCube>();

        GLCHECK(glEnable(GL_DEPTH_TEST));

        double deltaTime = 0.0;
        double lastFrame = 0.0;
        while (!window.ShouldClose())
        {
            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            if (window.IsKeyPressed(GLFW_KEY_ESCAPE))
            {
                window.Close();
                return 0;
            }

            camera.UpdateInput(window, (float)deltaTime);
            
            renderer.Clear();
            scene->Render(renderer, camera);

            window.SwapBuffers();
            glfwPollEvents();
        }
    }
    Engine::Logger::LogVerbose("Close window");
    return 0;
}
