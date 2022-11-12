#include <iostream>
#include <vector>

#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "bird.h"
#include "raylib.h"
#include "raymath.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 1280;

constexpr int FPS = 60;

constexpr int NUM_BIRDS = 50;

constexpr float BIRD_REACT_RADIUS = std::max(WIDTH, HEIGHT) / 4.0F;

void moveBirds(std::vector<boaf::Bird>& birds) {
  for (auto& bird : birds) {
    // Get nearby birds
    std::vector<boaf::Bird> nearby;

    for (const auto& other : birds) {
      float dist = Vector2Distance(bird.getPosition(), other.getPosition());
      if ((dist > 0) && (dist < BIRD_REACT_RADIUS)) {
        nearby.emplace_back(other);
      }
    }

    // React to our neighbors
    bird.react(nearby);
    LOG(INFO) << "Bird wants to go " << bird;
  }

  for (auto& bird : birds) {
    // Now move
    bird.move(WIDTH, HEIGHT);
    LOG(INFO) << "Bird went " << bird;
  }
}

void drawBirds(std::vector<boaf::Bird>& birds) {
  for (const auto& bird : birds) {
    bird.draw();
  }
}

int main(int argc, char* argv[]) {
  absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();

  InitWindow(WIDTH, HEIGHT, "Birds of a Feather");
  SetTargetFPS(FPS);

  std::vector<boaf::Bird> birds;
  birds.reserve(NUM_BIRDS);

  LOG(INFO) << "Attempting to create " << NUM_BIRDS << " birds.";
  for (int i = 0; i < NUM_BIRDS; i++) {
    birds.emplace_back(boaf::Bird::randomBird(WIDTH, HEIGHT));
    LOG(INFO) << "Created bird: " << birds[i];
  }

  while (!WindowShouldClose()) {
    moveBirds(birds);

    // Draw loop
    BeginDrawing();
    ClearBackground(DARKGRAY);
    drawBirds(birds);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
