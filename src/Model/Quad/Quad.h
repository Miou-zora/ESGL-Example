#pragma once

#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "Loader.h"
#include "Model.hpp"
#include "Core.hpp"
#include "Object.hpp"
#include "ShaderManager.hpp"


class Quad
{
public:
	Quad();
	~Quad() = default;

	void draw(glm::mat4 projection, glm::mat4 view, ES::Engine::Core &core);

private:
	Model model = Model();
	ES::Plugin::Object::Component::Transform transform = ES::Plugin::Object::Component::Transform();
	void generateMesh();
};
