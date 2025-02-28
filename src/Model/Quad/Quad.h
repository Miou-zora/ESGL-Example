#pragma once

#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "Loader.h"
#include "Mesh.hpp"
#include "Core.hpp"
#include "ShaderManager.hpp"


class Quad
{
public:
	Quad();
	~Quad() = default;

	void draw(glm::mat4 projection, glm::mat4 view, ES::Engine::Core &core);

private:
	Model model = Model();
	void generateMesh();
};
