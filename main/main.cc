#include <iostream>
#include <vector>

#include "absl/strings/str_join.h"
#include "raylib.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

constexpr float BIRD_RADIUS = 3;

std::vector<Color> bird_colors = {
    RED,
    GREEN,
    BLUE,
};

struct Bird {
 public:
  int x;
  int y;
  int color;
};

int wrap_width(int x) {
  while (x < 0) {
    x += WIDTH;
  }
  while (x >= WIDTH) {
    x -= WIDTH;
  }
  return x;
}

int wrap_height(int y) {
  while (y < 0) {
    y += HEIGHT;
  }
  while (y >= HEIGHT) {
    y -= HEIGHT;
  }
  return y;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> parts = {"Hello,", "world."};
  std::string greeting = absl::StrJoin(parts, " ");
  std::cout << greeting << std::endl;

  InitWindow(WIDTH, HEIGHT, "Birds of a Feather");

  std::vector<Bird> birds;

  for (int x = 0; x < WIDTH; x += 16) {
    for (int y = 0; y < HEIGHT; y += 16) {
      Bird b = {x, y, static_cast<int>((x + y) % bird_colors.size())};
      birds.emplace_back(b);
    }
  }

  while (!WindowShouldClose()) {
    // Update loop
    for (auto& bird : birds) {
      bird.x = wrap_width(bird.x + 1);
      bird.y = wrap_height(bird.y - 1);
    }

    // Draw loop
    BeginDrawing();
    ClearBackground(DARKGRAY);
    for (const auto& bird : birds) {
      DrawCircle(bird.x, bird.y, BIRD_RADIUS, bird_colors[bird.color]);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
