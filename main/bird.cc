//
// Created by cheilman on 11/11/22.
//

#include "bird.h"

#include <iostream>

#include "absl/log/log.h"

namespace boaf {

constexpr float BIRD_MAX_SPEED = 2;
constexpr float BIRD_RADIUS = 2;
constexpr float FLEE_FACTOR = 2;
constexpr float GROUP_FACTOR = 1;
constexpr float OTHER_FLOCK_AWAY_FACTOR = 2;
constexpr float AWAY_INVERSE_PROPORTION = 1.0F;
constexpr std::array<Color, 3> BIRD_COLORS = {RED, BLUE, GREEN};

std::ostream &operator<<(std::ostream &stream, const Vector2 &vec) {
  return stream << absl::StrFormat("(%0.2f, %02.f)", vec.x, vec.y);
}

std::ostream &operator<<(std::ostream &stream, const Bird &bird) {
  return stream << "Bird{" << bird.getColorIdx() << ", " << bird.getColorIdx()
                << bird.getPosition() << " -> " << bird.getHeading() << "}";
}

Vector2 Vector2Random(int maxX, int maxY) {
  return Vector2({static_cast<float>(GetRandomValue(0, maxX)),
                  static_cast<float>(GetRandomValue(0, maxY))});
}

Bird::Bird(Vector2 pos, int colorIdx)
    : _position(pos),
      _heading(Vector2Zero()),
      _colorIdx(colorIdx),
      _color(BIRD_COLORS.at(colorIdx)) {}

Bird Bird::randomBird(int maxX, int maxY) {
  return Bird(Vector2Random(maxX, maxY),
              GetRandomValue(0, BIRD_COLORS.size() - 1));
  //  return Bird(Vector2Random(maxX, maxY), 0);
}

void Bird::draw() const {
  DrawCircleV(this->_position, BIRD_RADIUS, this->_color);
  // TODO(cheilman): Make birds triangles w/ heading
  // DrawTriangle()
}

Vector2 Bird::flee(const std::vector<Bird> &nearbyBirds) const {
  Vector2 fleeVec = Vector2Zero();
  int count = 0;

  for (const auto &other : nearbyBirds) {
    Vector2 awayVec = Vector2Subtract(this->_position, other.getPosition());

    // Adjust by distance
    float dist = Vector2Distance(this->_position, other.getPosition());
    awayVec = Vector2Scale(
        awayVec, (static_cast<float>(count) * AWAY_INVERSE_PROPORTION) / dist);

    if (!this->isSameFlock(other)) {
      // Scale up unfriendly's awayVec
      awayVec = Vector2Scale(awayVec, OTHER_FLOCK_AWAY_FACTOR);
    }

    fleeVec = Vector2Add(fleeVec, awayVec);
    count++;
  }

  if (count > 0) {
    // Average it out
    fleeVec = Vector2Scale(fleeVec, 1.0F / static_cast<float>(count));
  }

  return fleeVec;
}

Vector2 Bird::group(const std::vector<Bird> &nearbyBirds) const {
  // Build centroid
  Vector2 centroid = Vector2Zero();
  int count = 0;

  for (const auto &other : nearbyBirds) {
    if (!this->isSameFlock(other)) {
      continue;
    }

    centroid = Vector2Add(centroid, other.getPosition());
    count++;
  }

  if (count <= 0) {
    return Vector2Zero();
  }

  centroid = Vector2Scale(centroid, 1.0F / static_cast<float>(count));
  Vector2 direction = Vector2Subtract(centroid, this->_position);

  return direction;
}

void Bird::react(const std::vector<Bird> &nearbyBirds) {
  // Move away from all birds
  Vector2 fleeVec = this->flee(nearbyBirds);

  // Move towards the centroid of our flock
  Vector2 groupVec = this->group(nearbyBirds);
  std::cerr << "Flee vec: " << fleeVec << ", Group vec: " << groupVec;

  // Weigh them differently
  fleeVec = Vector2Scale(fleeVec, FLEE_FACTOR);
  groupVec = Vector2Scale(groupVec, GROUP_FACTOR);
  std::cerr << "Flee vec: " << fleeVec << ", Group vec: " << groupVec;

  // Build the new heading
  Vector2 newHeading = Vector2Add(fleeVec, groupVec);
  Vector2ClampValue(newHeading, 0, BIRD_MAX_SPEED);

  this->_heading = newHeading;
}

void Bird::move(float maxX, float maxY) {
  this->_position = Vector2Add(
      this->_position, Vector2ClampValue(this->_heading, 0, BIRD_MAX_SPEED));
  this->_heading = Vector2Zero();

  // Wrap around
  while (this->_position.x > maxX) {
    this->_position.x -= maxX;
  }
  while (this->_position.x < 0) {
    this->_position.x += maxX;
  }
  while (this->_position.y > maxY) {
    this->_position.y -= maxY;
  }
  while (this->_position.y < 0) {
    this->_position.y += maxY;
  }
}

}  // namespace boaf