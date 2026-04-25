//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_DEMOWINDOW_H
#define VOXELENGINE_DEMOWINDOW_H
#include "ImguiDrawable.h"

namespace Ui {

class DemoWindow : public ImguiDrawable{
public:
    void Draw(double deltaTime) override;

private:
    bool open = true;
};

} // Ui

#endif //VOXELENGINE_DEMOWINDOW_H
