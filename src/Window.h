//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_WINDOW_H
#define VOXELENGINE_WINDOW_H
#include <string_view>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace Render {

class Window {
private:
    GLFWwindow* window;
    glm::ivec2 size;

public:
    Window(glm::ivec2 size, std::string_view title);
    [[nodiscard]] GLFWwindow* Get() const {return window;}
    [[nodiscard]] glm::ivec2 Size() const;
};

} // Render

#endif //VOXELENGINE_WINDOW_H
