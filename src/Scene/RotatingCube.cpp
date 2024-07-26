#include "RotatingCube.hpp"

#include "../IndexBuffer.hpp"
#include "../Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>

RotatingCube::RotatingCube()
{
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

    m_shader = std::make_unique<ShaderProgram>("ROTATING_CUBE");
    m_shader->Create(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
    m_shader->Use();

    m_vertexArrayObject = std::make_unique<VertexArrayObject>();
    m_vertexArrayObject->Bind();

    m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_vertexBuffer->DefineFloatAttribute(m_shader->GetAttribLocation("position"), 3);
    m_vertexBuffer->DefineFloatAttribute(m_shader->GetAttribLocation("color"), 3);
    m_vertexBuffer->DefineFloatAttribute(m_shader->GetAttribLocation("texureCoordinates"), 2);
    m_vertexArrayObject->AddBuffer(*m_vertexBuffer);

    m_colorBuffer = std::make_unique<VertexBuffer>(differentColors, sizeof(differentColors));
    m_colorBuffer->DefineFloatAttribute(m_shader->GetAttribLocation("color"), 3);
    m_vertexArrayObject->AddBuffer(*m_colorBuffer);

    m_indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

    m_texture = std::make_unique<Texture>("assets/flag.jpg");
    m_texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    m_texture->SetMinMag(GL_LINEAR, GL_LINEAR);
}

void RotatingCube::Render(Renderer& renderer, Camera& camera)
{
    m_shader->Use();
    m_vertexArrayObject->Bind();

    glm::mat4 model(1);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));

    m_shader->SetUniformMatrix4("model", model);
    m_shader->SetUniformMatrix4("view", camera.GetView());
    m_shader->SetUniformMatrix4("projection", camera.GetProjection());

    renderer.Draw(*m_vertexArrayObject, *m_indexBuffer, *m_shader);


}
