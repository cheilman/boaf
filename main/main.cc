#include <iostream>
#include <vector>

#include "absl/strings/str_join.h"
#include "raylib.h"
#include "raymath.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

constexpr float BIRD_RADIUS = 3;

constexpr float SAME_FLOCK_WEIGHT = 100000;
constexpr float SAME_FLOCK_RADIUS = 200;

constexpr float OTHER_FLOCK_WEIGHT = 2500;
constexpr float OTHER_FLOCK_RADIUS = 50;

constexpr float REPULSION_WEIGHT = 100000;
constexpr float REPULSION_RADIUS = 20;

constexpr float MAX_BIRD_VELOCITY = 4;

constexpr int RANDOM_BIRD_MOVEMENT = 10;

std::vector<Color> bird_colors = {
    RED,
    GREEN,
    BLUE,
};

struct Bird {
 public:
  Vector2 position;
  Vector2 heading;
  int color;

  Bird(float x, float y, int color)
      : position({x, y}), heading({0, 0}), color(color) {}
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

Vector2 move(Vector2 original, Vector2 heading) {
  Vector2 clampedHeading =
      Vector2ClampValue(heading, -1 * MAX_BIRD_VELOCITY, MAX_BIRD_VELOCITY);

  return Vector2Add(original, clampedHeading);
}

/**
 *
 * @param source
 * @param target
 * @param weight
 * @param radius
 * @param attract       If true move toward target, if false move away.
 * @return
 */
Vector2 moveRelativeTo(const Vector2& source, const Vector2& target, float dist,
                       float weight, float radius, bool attract) {
  if (dist > radius) {
    // No change
    return Vector2Zero();
  }

  float magnitude = weight / (dist * dist);
  Vector2 direction = Vector2Normalize(Vector2Subtract(target, source));
  if (!attract) {
    direction = Vector2Negate(direction);
  }

  return Vector2Scale(direction, magnitude);
}

/**
 * Figure out how one bird should react to another.
 *
 * @param bird
 * @param other
 * @return Vector showing how to move relative to the other bird.
 */
Vector2 react(const Bird& bird, const Bird& other) {
  float dist = Vector2Distance(bird.position, other.position);

  if (dist < REPULSION_RADIUS) {
    // Flee the bird
    return moveRelativeTo(bird.position, other.position, dist, REPULSION_WEIGHT,
                          REPULSION_RADIUS, false);
  } else if (bird.color == other.color) {
    // Attract to friends, if they're close
    return moveRelativeTo(bird.position, other.position, dist,
                          SAME_FLOCK_WEIGHT, SAME_FLOCK_RADIUS, true);
  } else if (bird.color != other.color) {
    // Flee enemies, if they're close
    return moveRelativeTo(bird.position, other.position, dist,
                          OTHER_FLOCK_WEIGHT, OTHER_FLOCK_RADIUS, false);
  }

  // No change.  Probably not reachable.
  return Vector2Zero();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> parts = {"Hello,", "world."};
  std::string greeting = absl::StrJoin(parts, " ");
  std::cout << greeting << std::endl;

  InitWindow(WIDTH, HEIGHT, "Birds of a Feather");

  std::vector<Bird> birds;

  for (int x = 0; x < WIDTH; x += 50) {
    for (int y = 0; y < HEIGHT; y += 50) {
      Bird b(x, y, GetRandomValue(0, 1));
      birds.emplace_back(b);
    }
  }

  unsigned long count = 0;

  while (!WindowShouldClose()) {
    // Update loop
    count++;

    // Figure out heading
    for (std::size_t i = 0; i < birds.size(); i++) {
      Bird& bird = birds[i];
      //      Vector2 target = {static_cast<float>(GetRandomValue(0,
      //      RANDOM_BIRD_MOVEMENT)), static_cast<float>(GetRandomValue(0,
      //      RANDOM_BIRD_MOVEMENT))}; Vector2 target = {0, 0};
      Vector2 target =
          Vector2Rotate({0, RANDOM_BIRD_MOVEMENT}, (count / 180.0f) * PI);
      int foundCount = 0;

      for (std::size_t j = 0; j < birds.size(); j++) {
        if (i == j) {
          continue;
        }
        const Bird& other = birds[j];

        Vector2 newHeading = react(bird, other);
        if ((newHeading.x != 0) || (newHeading.y != 0)) {
          target = Vector2Add(target, newHeading);
          foundCount++;
        }
      }

      if (foundCount > 0) {
        target = Vector2Scale(target, 1.0f / foundCount);
        bird.heading = target;
        std::cout << "Bird at (" << bird.position.x << ", " << bird.position.y
                  << ") moving -> (" << bird.heading.x << ", " << bird.heading.y
                  << ")" << std::endl;
      }
    }

    // Move birds
    for (auto& bird : birds) {
      bird.position = move(bird.position, bird.heading);
    }

    // Draw loop
    BeginDrawing();
    ClearBackground(DARKGRAY);
    for (const auto& bird : birds) {
      DrawCircleV(bird.position, BIRD_RADIUS, bird_colors[bird.color]);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
