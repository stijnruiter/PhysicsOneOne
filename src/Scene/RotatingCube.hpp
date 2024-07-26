#pragma once

#include "SceneBase.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Rendering/Texture.hpp"
#include <glm\glm.hpp>

#include <memory>

class RotatingCube : public SceneBase
{
private:
	std::unique_ptr<ShaderProgram> m_shader;
	std::unique_ptr<VertexArrayObject> m_vertexArrayObject;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<VertexBuffer> m_colorBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
	std::unique_ptr<Texture> m_texture;
public:
	RotatingCube();
	~RotatingCube() { }
	void Render(Renderer& renderer, Camera& camera);

};