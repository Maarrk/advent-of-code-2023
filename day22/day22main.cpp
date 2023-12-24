#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "day22.h"

using namespace std::literals;

int main(int argc, char *argv[]) {
    const auto input_filename = [&] {
        if (argc >= 2)
            return std::string{argv[1]};
        else
            return "example.txt"s;
    }();

    std::ifstream input_file{input_filename};
    if (input_file.fail()) {
        std::cerr << "Could not open " << input_filename << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Brick> bricks;
    {
        std::string line;
        while (!input_file.eof()) {
            std::getline(input_file, line);
            bricks.push_back(Brick::from_string(line));
        }
    }

    rl::BoundingBox bricks_bb;
    for (const auto b : bricks) {
        bricks_bb.min = b.start.Min(bricks_bb.min);
        bricks_bb.min = b.end.Min(bricks_bb.min);
        bricks_bb.max = b.start.Max(bricks_bb.max);
        bricks_bb.max = b.end.Max(bricks_bb.max);
    }
    bricks_bb.max = rl::Vector3::One() + bricks_bb.max;
    rl::Vector3 bricks_center =
        (rl::Vector3(bricks_bb.min) + rl::Vector3(bricks_bb.max)) / 2;
    float bricks_extents = bricks_center.Length() * 2;

    int screenWidth = 800;
    int screenHeight = 450;
    rl::Window window(screenWidth, screenHeight, "AoC 2023 - day 22");
    SetTargetFPS(60);

    rl::Camera camera(rl::Vector3::One() * bricks_extents, bricks_center,
                      rl::Vector3(0.0f, 0.0f, 1.0f), 45.0f, CAMERA_PERSPECTIVE);

    while (!window.ShouldClose()) {
        camera.Update(CAMERA_ORBITAL);

        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            camera.BeginMode();
            {
                for (const auto brick : bricks) {
                    brick.Draw();
                }
            }
            camera.EndMode();
        }
        EndDrawing();
    }

    return EXIT_SUCCESS;
}