#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"

// #include "Cow.h"
// #include "Sphere.h"
#include "Torus.h"
// #include "vboteapot.h"
// #include "Quad.h"
// #include "Plane.h"
// #include "Bunny.h"

#include "Viewer.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void initialize();
	void draw();
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
private:
	ShaderProgram* m_shaderProgram;
	// Cow* m_cow;
	// Sphere* m_sphere;
	VBOTorus* m_torus;
	// VBOTeapot* m_teapot;
	// Quad* m_quad;
	// Plane* m_plane;
	// Bunny* m_bunny;


	glm::vec2 m_size;
	float m_aspect;
	void setupBuffer();
};