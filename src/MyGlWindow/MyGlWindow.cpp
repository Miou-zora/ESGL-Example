#include "MyGlWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "Core.hpp"
#include "ShaderManager.hpp"

MyGlWindow::MyGlWindow(int w, int h, ES::Engine::Core &core) : m_size(w, h) {
	m_viewer = new Viewer(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, (float)w / h);
}


struct Light {
	glm::vec4 Position;
	glm::vec3 Intensity;
};

void MyGlWindow::UpdateMatrices(ES::Engine::Core &core)
{
	_view = glm::lookAt(m_viewer->getViewPoint(), m_viewer->getViewCenter(), m_viewer->getUpVector());
	_projection = glm::perspective(glm::radians(45.0f), m_size.x / m_size.y, 0.1f, 100.0f);
	_projectionView = _projection * _view;
}

void GLClearColor(ES::Engine::Core &core)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLClearDepth(ES::Engine::Core &core)
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GLEnableDepth(ES::Engine::Core &core)
{
	glEnable(GL_DEPTH_TEST);
}

void GLEnableCullFace(ES::Engine::Core &core)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MyGlWindow::SetupLights(ES::Engine::Core &core)
{
	core.GetResource<ShaderManager>().use("default");

	Light light[] = {
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.0f, 0.8f, 0.8f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.0f, 0.0f, 0.8f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.8f, 0.0f, 0.0f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.0f, 0.8f, 0.0f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.8f, 0.8f, 0.8f)}
	};

	float nbr_lights = 5.f;
	float scale = 2.f * glm::pi<float>() / nbr_lights;

	light[0].Position = glm::vec4( 5.f * cosf(scale * 0.f), 5.f, 5.f * sinf(scale * 0.f), 1.f);
	light[1].Position = glm::vec4( 5.f * cosf(scale * 1.f), 5.f, 5.f * sinf(scale * 1.f), 1.f);
	light[2].Position = glm::vec4( 5.f * cosf(scale * 2.f), 5.f, 5.f * sinf(scale * 2.f), 1.f);
	light[3].Position = glm::vec4( 5.f * cosf(scale * 3.f), 5.f, 5.f * sinf(scale * 3.f), 1.f);
	light[4].Position = glm::vec4( 5.f * cosf(scale * 4.f), 5.f, 5.f * sinf(scale * 4.f), 1.f);
	
	auto &shaderProgram = core.GetResource<ShaderManager>().get("default");
	for (int i = 0; i < 5; i++) {
		glUniform4fv(shaderProgram.uniform("Light[" + std::to_string(i) + "].Position"), 1, glm::value_ptr(light[i].Position));
		glUniform3fv(shaderProgram.uniform("Light[" + std::to_string(i) + "].Intensity"), 1, glm::value_ptr(light[i].Intensity));
	}

	glUniform3fv(shaderProgram.uniform("CamPos"), 1, glm::value_ptr(m_viewer->getViewPoint()));
	
	core.GetResource<ShaderManager>().disable("default");
}

void MyGlWindow::RenderMeshes(ES::Engine::Core &core)
{
	if (m_torus == nullptr) {
		m_torus = new VBOTorus();
	}
	if (m_torus) m_torus->draw(_projection, _view, core);
	if (m_quad == nullptr) {
		m_quad = new Quad();
	}
	if (m_quad) m_quad->draw(_projection, _view, core);
}

void MyGlWindow::draw(ES::Engine::Core &core) {
	UpdateMatrices(core);
	GLClearColor(core);
	GLClearDepth(core);
	GLEnableDepth(core);
	GLEnableCullFace(core);
	SetupLights(core);
	RenderMeshes(core);
}
