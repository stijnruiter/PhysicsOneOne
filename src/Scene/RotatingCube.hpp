#pragma once

#include "SceneBase.hpp"
#include "..\Texture.hpp"
#include <glm\glm.hpp>

class RotatingCube : public SceneBase
{
private:
	ShaderProgram* m_shader;
	VertexArrayObject* m_vertexArrayObject;
	VertexBuffer* m_vertexBuffer;
	VertexBuffer* m_colorBuffer;
	IndexBuffer* m_indexBuffer;
	Texture* m_texture;
public:
	RotatingCube();
	~RotatingCube();
	void Render(Renderer& renderer, Camera& camera);

};