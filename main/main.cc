#include <iostream>
#include <vector>

#include "absl/strings/str_join.h"
#include "raylib.h"

int main(int argc, char* argv[]) {
  std::vector<std::string> parts = {"Hello,", "world."};
  std::string greeting = absl::StrJoin(parts, " ");
  std::cout << greeting << std::endl;

  InitWindow(800, 640, "raylib [core] example - basic window");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats!  You created your first window!", 190, 200, 20,
             LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
