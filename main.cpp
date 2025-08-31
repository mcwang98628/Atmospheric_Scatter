
#include "Game.h"
#include <chrono>
#include <iostream>

Game game;

int main() {
    bool running = true;
    if (!game.Init())
    {
        return EXIT_FAILURE;
    }
    game.SetRunning(running);
    game.BindInput();
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    while (running)
    {
        std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();
        double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(currentFrameTime - lastFrameTime).count();
        float deltaTime = (float)(0.000000001 * duration);
        lastFrameTime = currentFrameTime;
        try {
            game.Update(deltaTime);
            game.DrawFrame();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    game.ShutDown();
    return EXIT_SUCCESS;
}