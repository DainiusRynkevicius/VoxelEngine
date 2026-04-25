//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Window.h"



    Window::Window(glm::ivec2 size, std::string_view title) : size(size){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
    }

    glm::ivec2 Window::Size() const {
        glm::ivec2 res;
        glfwGetFramebufferSize(window, &res.x, &res.y);
        return res;
    }
