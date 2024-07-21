#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        GLCHECK(glClear(GL_COLOR_BUFFER_BIT));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Engine::Logger::LogVerbose("Close window");
    glfwTerminate();
    return 0;
}
