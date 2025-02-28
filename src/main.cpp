/**
 * Project attended to report some issues from E²(https://github.com/EngineSquared/EngineSquared)
 * 
 * Issues:
 * - Can't report any errors from systems (like OpenGL errors). For example, initialization systems (like systems that use glfw functions that can return errors like "glfwInit") should raise an error if an error occured.
 * - We don't really know what we can do with Core.
 * - No clear way to run something when closing the program (like cleaning up resources, glfw context).
 * - We can't use Plugin::AssetsManager with string as key.
 */

#include <iostream>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "Core.hpp"
#include "Object.hpp"

#include "MyGlWindow.h"
#include "ShaderManager.hpp"
#include "MaterialCache.hpp"

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 800;

struct ESGLFWWINDOW
{
    GLFWwindow *window;
};

namespace ESGL {

    struct Button
    {
        bool pressed = false;
        bool updated = false;
    };

    struct Buttons
    {
        std::map<int, Button> mouse = {
            {GLFW_MOUSE_BUTTON_LEFT, Button()},
            {GLFW_MOUSE_BUTTON_RIGHT, Button()},
            {GLFW_MOUSE_BUTTON_MIDDLE, Button()}
        };
        glm::vec<2, double, glm::defaultp> lastMousePos = {0.0f, 0.0f};
        glm::vec<2, double, glm::defaultp> currentMousePos = {0.0f, 0.0f};
    };

    void UpdateKey(ES::Engine::Core &core)
    {
        GLFWwindow *window = core.GetResource<ESGLFWWINDOW>().window;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void UpdateButton(ES::Engine::Core &core)
    {
        GLFWwindow *window = core.GetResource<ESGLFWWINDOW>().window;
        auto &mouseButtons = core.GetResource<ESGL::Buttons>().mouse;
        for (auto &button : mouseButtons) {
            bool pressed = glfwGetMouseButton(window, button.first) == GLFW_PRESS;
            button.second.updated = button.second.pressed != pressed;
            button.second.pressed = pressed;
        }
    }

    void SaveLastMousePos(ES::Engine::Core &core)
    {
        auto &buttons = core.GetResource<ESGL::Buttons>();
        auto &lastMousePos = buttons.lastMousePos;
        auto &mouseButtons = buttons.mouse;
        auto window = core.GetResource<ESGLFWWINDOW>().window;
        if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT].updated ||
            mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE].updated ||
            mouseButtons[GLFW_MOUSE_BUTTON_RIGHT].updated) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastMousePos.x = xpos;
            lastMousePos.y = ypos;
        }
    }

    void InitGLFW(ES::Engine::Core &core)
    {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
    }

    void SetupGLFWHints(ES::Engine::Core &core)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void CreateGLFWWindow(ES::Engine::Core &core)
    {
        if (!core.RegisterResource<ESGLFWWINDOW>({glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "OpenGL Framework", NULL, NULL)}).window) {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return;
        }
    }

    void LinkGLFWContextToGL(ES::Engine::Core &core)
    {
        glfwMakeContextCurrent(core.GetResource<ESGLFWWINDOW>().window);
    }

    void InitGL3W(ES::Engine::Core &core)
    {
        if (gl3wInit()) {
            std::cerr << "Failed to initialize OpenGL" << std::endl;
            return;
        }
    }

    void CheckGL3WVersion(ES::Engine::Core &core)
    {
        if (!gl3wIsSupported(4, 2)) {
            std::cerr << "OpenGL 4.2 not supported" << std::endl;
            return;
        }
    }

    void GLFWEnableVSync(ES::Engine::Core &core)
    {
        glfwSwapInterval(1);
    }

    void InitCustomWindow(ES::Engine::Core &core)
    {
        core.RegisterResource<MyGlWindow>(MyGlWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, core));
    }

    void UpdatePosCursor(ES::Engine::Core &core)
    {
        auto &currentMousePos = core.GetResource<ESGL::Buttons>().currentMousePos;
        glfwGetCursorPos(core.GetResource<ESGLFWWINDOW>().window, &currentMousePos.x, &currentMousePos.y);
    }

    // Function to handle mouse dragging interactions
    void MouseDragging(ES::Engine::Core &core)
    {
        auto &buttons = core.GetResource<ESGL::Buttons>();
        auto &lastMousePos = buttons.lastMousePos;
        auto &currentMousePos = buttons.currentMousePos;
        auto &mouseButtons = buttons.mouse;
        MyGlWindow &window = core.GetResource<MyGlWindow>();
        if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT].pressed) {
            float fractionChangeX = static_cast<float>(currentMousePos.x - lastMousePos.x) / static_cast<float>(window.getSize().x);
            float fractionChangeY = static_cast<float>(lastMousePos.y - currentMousePos.y) / static_cast<float>(window.getSize().y);
            window.getViewer()->rotate(fractionChangeX, fractionChangeY);
        }
        else if (mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE].pressed) {
            float fractionChangeY = static_cast<float>(lastMousePos.y - currentMousePos.y) / static_cast<float>(window.getSize().y);
            window.getViewer()->zoom(fractionChangeY);
        }
        else if (mouseButtons[GLFW_MOUSE_BUTTON_RIGHT].pressed) {
            float fractionChangeX = static_cast<float>(currentMousePos.x - lastMousePos.x) / static_cast<float>(window.getSize().x);
            float fractionChangeY = static_cast<float>(lastMousePos.y - currentMousePos.y) / static_cast<float>(window.getSize().y);
            window.getViewer()->translate(-fractionChangeX, -fractionChangeY, true);
        }
        lastMousePos.x = currentMousePos.x;
        lastMousePos.y = currentMousePos.y;
    }

    void Draw(ES::Engine::Core &core)
    {
        core.GetResource<MyGlWindow>().draw(core);
    }

    void SwapBuffers(ES::Engine::Core &core)
    {
        glfwSwapBuffers(core.GetResource<ESGLFWWINDOW>().window);
    }
    
    void PollEvents(ES::Engine::Core &core)
    {
        glfwPollEvents();
    }
    
    void LoadShaderManager(ES::Engine::Core &core)
    {
        ShaderManager &shaderManager = core.RegisterResource<ShaderManager>(ShaderManager());
        ShaderProgram &program = shaderManager.add("default");
        program.initFromFiles("shaders/simple.vert", "shaders/simple.frag");
    }

    void SetupShaderUniforms(ES::Engine::Core &core) {
        auto &m_shaderProgram = core.GetResource<ShaderManager>().get("default");
    
        // Add uniforms
        m_shaderProgram.addUniform("MVP");
        m_shaderProgram.addUniform("ModelMatrix"); //View*Model : mat4
        m_shaderProgram.addUniform("NormalMatrix"); //Refer next slide : mat3
    
        for (int i = 0; i < 5; i++) {
            m_shaderProgram.addUniform("Light[" + std::to_string(i) + "].Position");
            m_shaderProgram.addUniform("Light[" + std::to_string(i) + "].Intensity");
        }
        m_shaderProgram.addUniform("Material.Ka");
        m_shaderProgram.addUniform("Material.Kd");
        m_shaderProgram.addUniform("Material.Ks");
        m_shaderProgram.addUniform("Material.Shiness");
    
        m_shaderProgram.addUniform("CamPos");
    }

    void LoadMaterialCache(ES::Engine::Core &core)
    {
        auto &materialCache = core.RegisterResource<MaterialCache>({});
        materialCache.add("default");
    }
}


int main()
{
    ES::Engine::Core core;

    core.RegisterResource<ESGL::Buttons>({});

    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::InitGLFW);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::SetupGLFWHints);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::CreateGLFWWindow);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::LinkGLFWContextToGL);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::InitGL3W);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::CheckGL3WVersion);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::GLFWEnableVSync);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::InitCustomWindow);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::LoadMaterialCache);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::LoadShaderManager);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::SetupShaderUniforms);

    core.RunSystems();

    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::UpdateKey);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::UpdatePosCursor);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::UpdateButton);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::SaveLastMousePos);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::Draw);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::SwapBuffers);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::PollEvents);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::MouseDragging);

    while (!glfwWindowShouldClose(core.GetResource<ESGLFWWINDOW>().window)) {
        core.RunSystems();
    }

    glfwDestroyWindow(core.GetResource<ESGLFWWINDOW>().window);
    glfwTerminate();

    return 0;
}
