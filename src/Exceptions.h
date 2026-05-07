//
// Created by Dainius Rynkevicius on 07/05/2026.
//

#ifndef VOXELENGINE_EXCEPTIONS_H
#define VOXELENGINE_EXCEPTIONS_H
#include <exception>

#include "glm/vec3.hpp"

class InvalidChunkPositionException : public std::exception {
public:
    glm::ivec3 invalid;

    explicit InvalidChunkPositionException(const glm::ivec3 invalid) : invalid(invalid) {
    }
};

#endif //VOXELENGINE_EXCEPTIONS_H
