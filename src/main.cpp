/**
 * Project attended to report some issues from E²(https://github.com/EngineSquared/EngineSquared)
 * 
 * Issues:
 * - Can't report any errors from systems (like OpenGL errors). For example, initialization systems (like InitRenderer) should raise an error if an error occured.
 * - We don't really know what we can do with Core.
 */

#include <iostream>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "Core.hpp"

#include "MyGlWindow.h"

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 800;

struct ESGLFWWINDOW
{
    GLFWwindow *window;
};

namespace ESGL {

    struct Buttons
    {
        std::map<int, bool> mouse = {
            {GLFW_MOUSE_BUTTON_LEFT, false},
            {GLFW_MOUSE_BUTTON_RIGHT, false},
            {GLFW_MOUSE_BUTTON_MIDDLE, false}
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

    // replicate mouse_button_callback
    void UpdateButton(ES::Engine::Core &core)
    {
        GLFWwindow *window = core.GetResource<ESGLFWWINDOW>().window;
        auto &buttons = core.GetResource<ESGL::Buttons>();
        auto &lastMousePos = buttons.lastMousePos;
        auto &mouseButtons = buttons.mouse;
        static bool last_state_lbutton = false;
        static bool last_state_rbutton = false;
        static bool last_state_mbutton = false;
        mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        mouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
        mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
        
        bool pressed = false;
        pressed = mouseButtons[GLFW_MOUSE_BUTTON_LEFT] != last_state_lbutton;
        last_state_lbutton = mouseButtons[GLFW_MOUSE_BUTTON_LEFT];
        pressed = mouseButtons[GLFW_MOUSE_BUTTON_RIGHT] != last_state_rbutton;
        last_state_rbutton = mouseButtons[GLFW_MOUSE_BUTTON_RIGHT];
        pressed = mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] != last_state_mbutton;
        last_state_mbutton = mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE];

        if (pressed) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastMousePos.x = xpos;
            lastMousePos.y = ypos;
        }
    }

    void InitRenderer(ES::Engine::Core &core)
    {
        // Initialize the library
        if (!glfwInit()) {
            // Initialization failed
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a windowed mode window and its OpenGL context
        GLFWwindow *glfw_window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "OpenGL Framework", NULL, NULL);
        if (!glfw_window) {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return;
        }
        ESGLFWWINDOW &es_glfw_window = core.RegisterResource<ESGLFWWINDOW>({glfw_window});

        glfwMakeContextCurrent(es_glfw_window.window);
        if (gl3wInit()) {
            std::cerr << "Failed to initialize OpenGL" << std::endl;
            return;
        }

        if (!gl3wIsSupported(4, 2)) {
            std::cerr << "OpenGL 4.2 not supported" << std::endl;
            return;
        }

        glfwSwapInterval(1); // Enable vsync

        core.RegisterResource<MyGlWindow>(MyGlWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT));
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
        if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
            float fractionChangeX = static_cast<float>(currentMousePos.x - lastMousePos.x) / static_cast<float>(window.getSize().x);
            float fractionChangeY = static_cast<float>(lastMousePos.y - currentMousePos.y) / static_cast<float>(window.getSize().y);
            window.getViewer()->rotate(fractionChangeX, fractionChangeY);
        }
        else if (mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE]) {
            float fractionChangeY = static_cast<float>(lastMousePos.y - currentMousePos.y) / static_cast<float>(window.getSize().y);
            window.getViewer()->zoom(fractionChangeY);
        }
        else if (mouseButtons[GLFW_MOUSE_BUTTON_RIGHT]) {
            float fractionChangeX = static_cast<float>(currentMousePos.x - lastMousePos.x) / static_cast<float>(window.getSize().x);
            float fractionChangeY = static_cast<float>(lastMousePos.y - currentMousePos.y) / static_cast<float>(window.getSize().y);
            window.getViewer()->translate(-fractionChangeX, -fractionChangeY, true);
        }
        lastMousePos.x = currentMousePos.x;
        lastMousePos.y = currentMousePos.y;
    }

    void Draw(ES::Engine::Core &core)
    {
        core.GetResource<MyGlWindow>().draw();
    }

    void SwapBuffers(ES::Engine::Core &core)
    {
        glfwSwapBuffers(core.GetResource<ESGLFWWINDOW>().window);
    }
    
    void PollEvents(ES::Engine::Core &core)
    {
        glfwPollEvents();
    }
}

int main()
{
    ES::Engine::Core core;
    
    core.RegisterResource<ESGL::Buttons>({});

    core.RegisterSystem<ES::Engine::Scheduler::Startup>(ESGL::InitRenderer);

    core.RunSystems();

    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::UpdateKey);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::UpdatePosCursor);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::UpdateButton);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::Draw);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::SwapBuffers);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::PollEvents);
    core.RegisterSystem<ES::Engine::Scheduler::Update>(ESGL::MouseDragging);

    while (!glfwWindowShouldClose(core.GetResource<ESGLFWWINDOW>().window)) {
        core.RunSystems();
    }

    return 0;
}
