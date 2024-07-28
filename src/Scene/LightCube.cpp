#include "LightCube.hpp"

#include <glm/gtc/matrix_transform.hpp>

LightCube::LightCube()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //0
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // 4
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, //8
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //12
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, //16
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //20
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    // Create cube buffers
    m_vertexArrayObject = std::make_unique<VertexArrayObject>();
    m_vertexArrayObject->Bind();

    m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_vertexBuffer->DefineFloatAttribute(0, 3); // TODO: ASSERT both shaders use position at the same location
    m_vertexBuffer->DefineFloatAttribute(1, 3); // TODO: ASSERT both shaders use position at the same location
    m_vertexArrayObject->AddBuffer(*m_vertexBuffer);

    m_indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

    // Create const light shader
    m_shaderConstLight = std::make_unique<ShaderProgram>("CONST_LIGHT_CUBE");
    m_shaderConstLight->Create(VERTEX_SHADER_POS_MVP, FRAGMENT_SHADER_UNIFORM_COLOR);
    m_shaderConstLight->Use();
    m_shaderConstLight->SetUniformVector4("color", glm::vec4(1.0f));

    // Create variable light shader
    m_shaderVariableLight = std::make_unique<ShaderProgram>("VARIABLE_LIGHT_CUBE");
    m_shaderVariableLight->Create(VERTEX_SHADER_POS_NORMAL_MVP, FRAGMENT_SHADER_VARIABLE_COLOR);
    m_shaderVariableLight->Use();
    m_shaderVariableLight->SetUniformVector4("lightColor", glm::vec4(0.5f));
    m_shaderVariableLight->SetUniformVector4("objectColor", glm::vec4(1.0f));
}

void LightCube::Render(Renderer& renderer, Camera& camera)
{
    // Draw variable light cube
    m_shaderVariableLight->Use();
    m_vertexArrayObject->Bind();
    glm::mat4 model(1);
    m_shaderVariableLight->SetUniformMatrix4("model", model);
    m_shaderVariableLight->SetUniformMatrix4("view", camera.GetView());
    m_shaderVariableLight->SetUniformMatrix4("projection", camera.GetProjection());
    renderer.Draw(*m_vertexArrayObject, *m_indexBuffer, *m_shaderVariableLight);

    // Draw const light cube
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    m_shaderConstLight->Use();
    m_shaderConstLight->SetUniformMatrix4("model", model);
    m_shaderConstLight->SetUniformMatrix4("view", camera.GetView());
    m_shaderConstLight->SetUniformMatrix4("projection", camera.GetProjection());
    renderer.Draw(*m_vertexArrayObject, *m_indexBuffer, *m_shaderConstLight);
}
