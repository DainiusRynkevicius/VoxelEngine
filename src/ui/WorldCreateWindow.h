//
// Created by daini on 05/05/2026.
//

#ifndef VOXELENGINE_WORLDCREATEWINDOW_H
#define VOXELENGINE_WORLDCREATEWINDOW_H
#include "ImguiDrawable.h"
#include "../Application.h"
#include "../world/generators/FlatGenerator.h"

namespace Ui {
    class WorldCreateWindow : public ImguiDrawable{
    public:
        WorldCreateWindow(Application& application, World::Blocks::BlockRegistry& registry) : application(application), registry(registry){}
        void Draw(double deltaTime) override;

        bool open = true;

    private:
        Application& application;
        World::Blocks::BlockRegistry& registry;
        int generator_selected = 0;
        std::vector<std::string> generators = {
            "Flat Generator",
        };

        glm::ivec3 level_size = {3, 3, 3};

        void GeneratorSelector();
        void GeneratorOptions();
        //TODO: maybe do this better?
        // Generator options
        World::Generators::FlatGenerator::Options flat;
    };
} // Ui

#endif //VOXELENGINE_WORLDCREATEWINDOW_H
