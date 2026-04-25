#include <iostream>

#include "Application.h"
#include "spdlog/spdlog.h"

int main() {
    Application app{};
    try {
        app.Run();
    } catch (std::exception& e) {
        spdlog::error("Uncaught exception: {}", e.what());
        throw;
    }

    return 0;
}
