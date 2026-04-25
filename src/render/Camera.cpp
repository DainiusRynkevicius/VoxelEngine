//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#include "Camera.h"

namespace Render {
    glm::mat4 Camera::View() {
        return glm::lookAt(position, position + Forward(), Up());
    }

    glm::mat4 Camera::Projection(float aspect_ratio) {
        return glm::infinitePerspectiveRH_ZO(glm::radians(70.0f), aspect_ratio, 0.01f);
    }
} // Render
