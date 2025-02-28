#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"

// #include "Cow.h"
// #include "Sphere.h"
#include "Torus.h"
// #include "vboteapot.h"
#include "Quad.h"
// #include "Plane.h"
// #include "Bunny.h"
#include "Core.hpp"

#include "Viewer.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h, ES::Engine::Core &core);
	void initialize();
	void draw(ES::Engine::Core &core);
	Viewer* m_viewer;
	Viewer* getViewer() const { return m_viewer; };
	void setSize(int width, int height) {
		m_size.x = width;
		m_size.y = height;
	};
	glm::vec2 getSize(void)
	{
		return m_size;
	}
	void setAspect(float aspect) {
		m_aspect = aspect;
		m_viewer->setAspectRatio(aspect);
	};
	void UpdateMatrices(ES::Engine::Core &core);
	void SetupLights(ES::Engine::Core &core);
	void RenderMeshes(ES::Engine::Core &core);
private:
	// Cow* m_cow;
	// Sphere* m_sphere;
	VBOTorus* m_torus = nullptr;
	// VBOTeapot* m_teapot;
	Quad* m_quad = nullptr;
	// Plane* m_plane;
	// Bunny* m_bunny;

	glm::mat4 _view;
	glm::mat4 _projection;
	glm::mat4 _projectionView;

	glm::vec2 m_size;
	float m_aspect;
};