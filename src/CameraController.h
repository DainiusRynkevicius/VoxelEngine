//
// Created by Dainius Rynkevicius on 02/05/2026.
//

#ifndef VOXELENGINE_CAMERACONTROLLER_H
#define VOXELENGINE_CAMERACONTROLLER_H
#include "GLFW/glfw3.h"
#include "render/Camera.h"


class CameraController {
public:
    explicit CameraController(Render::Camera& camera);

    void HandleInput(float delta, GLFWwindow* window);

private:
    void HandleMovement(float delta, GLFWwindow* window);

    void HandleLook(GLFWwindow* window);

    Render::Camera& camera;

    float move_speed = 8.0f;
    float look_speed = 1.0f;

    float yaw = 0.0f;
    float pitch = 0.0f;

    bool look_active = false;
    glm::vec2 last_mouse_pos{0.0f};

    glm::vec3 Forward() const {
        return camera.rotation * glm::vec3{0.0f, 0.0f, -1.0f};
    }

    glm::vec3 Right() const {
        return camera.rotation * glm::vec3{1.0f, 0.0f, 0.0f};
    }

    glm::vec3 Up() const {
        return camera.rotation * glm::vec3{0.0f, 1.0f, 0.0f};
    }
};


#endif //VOXELENGINE_CAMERACONTROLLER_H
