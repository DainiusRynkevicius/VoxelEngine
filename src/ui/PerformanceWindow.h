//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_FPSCOUNTERWINDOW_H
#define VOXELENGINE_FPSCOUNTERWINDOW_H
#include <array>

#include "ImguiDrawable.h"


namespace Ui {
    class PerformanceWindow : public Ui::ImguiDrawable{
        void Draw(double deltaTime) override;

    private:
        static constexpr size_t SAMPLE_SIZE = 100;
        bool open = true;
        std::array<float, SAMPLE_SIZE> frame_times{};
        int offset = 0;
    };
}




#endif //VOXELENGINE_FPSCOUNTERWINDOW_H
