#include <fstream>
#include <iostream>
#include <string>

#include "day22.h"

using namespace std::literals;

int main() {
  int screenWidth = 800;
  int screenHeight = 450;

  raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");
  raylib::Texture logo("raylib_logo.png");

  SetTargetFPS(60);

  while (!window.ShouldClose()) {
    BeginDrawing();

    window.ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    // Object methods.
    logo.Draw(screenWidth / 2 - logo.GetWidth() / 2,
              screenHeight / 2 - logo.GetHeight() / 2);

    EndDrawing();
  }

  return EXIT_SUCCESS;
}