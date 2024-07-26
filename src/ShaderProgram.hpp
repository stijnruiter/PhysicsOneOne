#pragma once

#include <glad\glad.h>
#include <string>
#include <glm/glm.hpp>


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

class ShaderProgram
{
private:
	std::string debugIdentifier;
	unsigned int programId;

	unsigned int LoadFromSource(const GLenum shaderType, const std::string& source) const;
	bool CheckShaderCompileStatus(const unsigned int shaderId) const;
	bool CheckShaderProgramLinkStatus(const unsigned int shaderProgramId) const;

public:
	ShaderProgram(const std::string& debugIdentifier = "SHADERPROGRAM");
	~ShaderProgram();

	void Create(const std::string& vertexSource, const std::string& fragmentSource);

	int GetAttribLocation(const std::string& attribName);
	int GetUniformLocation(const std::string& uniformName);

	void SetUniformMatrix4(const std::string& uniformName, const glm::mat4& mat);

	void Use() const;
	void Unuse() const;
};