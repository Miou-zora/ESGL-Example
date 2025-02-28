#include "Quad.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "MaterialCache.hpp"
#include <iostream>

Quad::Quad()
{
	// Generate the vertex data
	generateMesh();

	model.generateGlBuffers();
}

void Quad::draw(glm::mat4 projection, glm::mat4 view, ES::Engine::Core &core) 
{
	auto &shader = core.GetResource<ShaderManager>().get(model.shaderName);
	const auto material = core.GetResource<MaterialCache>().get(model.materialName);
	shader.use();
	glUniform3fv(shader.uniform("Material.Ka"), 1, glm::value_ptr(material.Ka));
	glUniform3fv(shader.uniform("Material.Kd"), 1, glm::value_ptr(material.Kd));
	glUniform3fv(shader.uniform("Material.Ks"), 1, glm::value_ptr(material.Ks));
	glUniform1fv(shader.uniform("Material.Shiness"), 1, &material.Shiness);
	glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelmat = glm::rotate(modelmat, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	modelmat = glm::scale(modelmat, glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 mview = view * modelmat;
	glm::mat4 mvp = projection * view * modelmat;
	glm::mat4 imvp = glm::inverse(modelmat);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader.uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader.uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelmat));
	glUniformMatrix4fv(shader.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	model.draw();
	shader.disable();
}

void Quad::generateMesh()
{
	using namespace glm;

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec<3, unsigned int>> triIndices;

	vertices = {
		vec3(-1,  1, 0),
		vec3( 1,  1, 0),
		vec3(-1, -1, 0),
		vec3( 1, -1, 0)
	};

	normals = {
		vec3(0, 0, -1),
		vec3(0, 0, -1),
		vec3(0, 0, -1),
		vec3(0, 0, -1)
	};

	triIndices = {
		{2, 0, 1},
		{2, 1, 3}
	};

	model.vertices = vertices;
	model.normals = normals;
	model.triIndices = triIndices;
	model.shaderName = "default";
	model.materialName = "default";
}

