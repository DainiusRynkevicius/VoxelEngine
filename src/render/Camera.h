//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_CAMERA_H
#define VOXELENGINE_CAMERA_H

#include <glm/glm.hpp>

#include "glm/detail/type_quat.hpp"

namespace Render {
    class Camera {
    public:
        glm::vec3 position;
        glm::quat rotation;

        glm::mat4 View();

        glm::mat4 Projection(float aspect_ratio);

        glm::vec3 Forward() {
            return rotation * glm::vec3{0, 0, -1};
        }

        glm::vec3 Up() {
            return rotation * glm::vec3{0, 1, 0};
        }
    };
} // Render

#endif //VOXELENGINE_CAMERA_H
