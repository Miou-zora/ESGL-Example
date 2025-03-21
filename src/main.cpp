/**
 * Project attended to report some issues from E²(https://github.com/EngineSquared/EngineSquared)
 *
 * Issues:
 * - Can't report any errors from systems (like OpenGL errors). For example,
 * initialization systems (like systems that use glfw functions that can return
 * errors like "glfwInit") should raise an error if an error occured.
 * - We don't really know what we can do with Core.
 * - No clear way to run something when closing the program (like cleaning up resources, glfw context).
 * - We can't use Plugin::AssetsManager with string as key.
 * - Mesh of E² is too vague and not really useful.
 * - Local plugin and global xmake require to put the hard path of the engine.
 */

#include "Core.hpp"
#include "Entity.hpp"
#include "OpenGL.hpp"
#include "Startup.hpp"
#include "Window.hpp"

void TESTAddQuad(ES::Engine::Core &core)
{
    using namespace glm;

    auto quad = ES::Engine::Entity(core.GetRegistry().create());

    ES::Plugin::OpenGL::Component::Model model;

    model.shaderName = "default";
    model.materialName = "default";
    model.glbufferName = "default";

    ES::Plugin::Object::Component::Mesh mesh;

    mesh.vertices = {ES::Plugin::Object::Component::Vertex(glm::vec3(-1, 1, 0), glm::vvec3(0, 0, -1)),
                ES::Plugin::Object::Component::Vertex(glm::vvec3(1, 1, 0), glm::vvec3(0, 0, -1)),
                ES::Plugin::Object::Component::Vertex(vglm::vec3(-1, -1, 0), glm::vvec3(0, 0, -1)),
                ES::Plugin::Object::Component::Vertex(glm::vvec3(1, -1, 0), glm::vvec3(0, 0, -1))};

    mesh.indices = {2, 0, 1, 2, 1, 3};

    model.generateGlBuffers(mesh);

    quad.AddComponent<ES::Plugin::OpenGL::Component::Model>(core, model);
    auto &transform = quad.AddComponent<ES::Plugin::Object::Component::Transform>(core, {});

    transform.position = glm::vec3(0.0f, -1.0f, 0.0f);
    transform.rotation = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    transform.scale = glm::vec3(10.0f, 10.0f, 10.0f);
}

void TESTGenerateData(ES::Plugin::Object::Component::Mesh &mesh, float outerRadius, float innerRadius)
{
    using namespace glm;

    mesh.vertices.reserve(100 * 100);
    mesh.indices.reserve(100 * 100 * 6);

    float TWOPI = 2 * glm::pi<float>();

    float ringFactor  = (float)(TWOPI / 100);
    float sideFactor = (float)(TWOPI / 100);
    int idx = 0, tidx = 0;

    for( int ring = 0; ring <= 100; ring++ ) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);
        for( int side = 0; side < 100; side++ ) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);
            float r = (outerRadius + innerRadius * cv);
            // Normalize
            float len = sqrt( mesh.normals[idx].x * mesh.normals[idx].x +
                            mesh.normals[idx].y * mesh.normals[idx].y +
                            mesh.normals[idx].z * mesh.normals[idx].z );
            glm::vec3 normal = vec3(cv * cu * r, cv * su * r, sv * r) / len;
            mesh.vertices.emplace_back(ES::Plugin::Object::Component::Vertex(vec3(r * cu, r * su, innerRadius * sv), normal));
            idx += 1;
        }
    }

    idx = 0;
    for( int ring = 0; ring < 100; ring++ ) {
        int ringStart = ring * 100;
        int nextRingStart = (ring + 1) * 100;
        for( int side = 0; side < 100; side++ ) {
            int nextSide = (side+1) % 100;
            // The quad
            mesh.indices.insert(mesh.indices.end(), {ringStart + side, nextRingStart + side, nextRingStart + nextSide});
            mesh.indices.insert(mesh.indices.end(), {ringStart + side, nextRingStart + nextSide, ringStart + nextSide});
        }
    }
}

void TESTAddTorus(ES::Engine::Core &core)
{
    using namespace glm;

    auto torus = ES::Engine::Entity(core.GetRegistry().create());
    auto &mat = core.GetResource<ES::Plugin::OpenGL::Resource::MaterialCache>().Add(entt::hashed_string("TESTTorus"), std::move(ES::Plugin::OpenGL::Utils::Material()));
    mat.Shiness = 180.0f;
    mat.Ka = vec3(0.1, 0.1, 0.1);
    mat.Kd = vec3(0.4, 0.4, 0.4);
    mat.Ks = vec3(0.9,0.9, 0.9);

    ES::Plugin::OpenGL::Component::Model model;

    model.shaderName = "default";
    model.materialName = "TESTTorus";
    model.glbufferName = "TESTTorus";

    ES::Plugin::Object::Component::Mesh mesh;

    TESTGenerateData(mesh, 1.5f, 0.3f);
    model.generateGlBuffers(mesh);

    torus.AddComponent<ES::Plugin::OpenGL::Component::Model>(core, model);
    auto &transform = torus.AddComponent<ES::Plugin::Object::Component::Transform>(core, {});
    transform.rotation = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
}

int main()
{
    ES::Engine::Core core;

    core.AddPlugins<ES::Plugin::OpenGL::Plugin>();

    core.RegisterSystem<ES::Engine::Scheduler::Startup>(TESTAddQuad);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(TESTAddTorus);

    core.RunCore();

    glfwDestroyWindow(core.GetResource<ES::Plugin::Window::Resource::Window>().GetGLFWWindow());
    glfwTerminate();

    return 0;
}
