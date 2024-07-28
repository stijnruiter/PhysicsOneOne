#pragma once

#include "SceneBase.hpp"
#include <memory>

class LightCube : public SceneBase
{
private:
	std::unique_ptr<ShaderProgram> m_shaderConstLight;
	std::unique_ptr<ShaderProgram> m_shaderVariableLight;
	std::unique_ptr<VertexArrayObject> m_vertexArrayObject;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<VertexBuffer> m_vertexNormalBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
public:
	LightCube();
	~LightCube() { }
	void Render(Renderer& renderer, Camera& camera);
};