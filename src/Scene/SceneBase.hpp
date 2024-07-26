#pragma once

#include "../Rendering/Renderer.hpp"
#include "../Camera.hpp"

class SceneBase
{
protected:

public:
	virtual void Render(Renderer& renderer, Camera& camera) = 0;
	virtual ~SceneBase() { }
};