#include "CameraController.h"

#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

CameraController::CameraController(Render::Camera& camera)
    : camera(camera) {
}

void CameraController::HandleInput(float delta, GLFWwindow* window) {
    HandleLook(window);
    HandleMovement(delta, window);
}

void CameraController::HandleMovement(float delta, GLFWwindow* window) {
    glm::vec3 move{0.0f};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        move += Forward();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        move -= Forward();
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        move += Right();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        move -= Right();
    }

    constexpr glm::vec3 world_up{0.0f, 1.0f, 0.0f};

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        move += world_up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        move -= world_up;
    }

    if (glm::length2(move) > 0.0f) {
        camera.position += glm::normalize(move) * move_speed * delta;
    }
}

void CameraController::HandleLook(GLFWwindow* window) {
    bool should_look =
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (should_look && !look_active) {
        look_active = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        last_mouse_pos = {
            static_cast<float>(x),
            static_cast<float>(y)
        };
    }

    if (!should_look && look_active) {
        look_active = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

    if (!look_active) {
        return;
    }

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    glm::vec2 mouse_pos{
        static_cast<float>(mouse_x),
        static_cast<float>(mouse_y)
    };

    glm::vec2 mouse_delta = mouse_pos - last_mouse_pos;
    last_mouse_pos = mouse_pos;

    constexpr float sensitivity = 0.001f;

    yaw -= mouse_delta.x * look_speed * sensitivity;
    pitch -= mouse_delta.y * look_speed * sensitivity;

    constexpr float max_pitch = glm::radians(89.0f);
    pitch = std::clamp(pitch, -max_pitch, max_pitch);

    glm::quat yaw_rot =
            glm::angleAxis(yaw, glm::vec3{0.0f, 1.0f, 0.0f});

    glm::quat pitch_rot =
            glm::angleAxis(pitch, glm::vec3{1.0f, 0.0f, 0.0f});

    camera.rotation = glm::normalize(yaw_rot * pitch_rot);
}
