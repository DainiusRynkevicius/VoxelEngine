//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_FPSCOUNTERWINDOW_H
#define VOXELENGINE_FPSCOUNTERWINDOW_H
#include "ImguiDrawable.h"


namespace Ui {
    class PerformanceWindow : public Ui::ImguiDrawable{
        void Draw(double deltaTime) override;
    };
}




#endif //VOXELENGINE_FPSCOUNTERWINDOW_H
