//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Window.h"

namespace Render {
    Window::Window(glm::ivec2 size, std::string_view title) : size(size){
        glfwInitHint(GLFW_NO_API, GLFW_TRUE);
        glfwInit();

        window = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
    }

    glm::ivec2 Window::Size() const {
        glm::ivec2 res;
        glfwGetWindowSize(window, &res.x, &res.y);
        return res;
    }
} // Render