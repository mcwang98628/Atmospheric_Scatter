
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
    while (running)
    {
        float deltaTime = 0;
        std::chrono::high_resolution_clock::time_point frameEnd;
        std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
        try {
            frameEnd = std::chrono::high_resolution_clock::now();
            double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count();
            deltaTime = (float)(0.000000001 * duration);

            // game.ProcessInput();
            frameStart = frameEnd;
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