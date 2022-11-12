//
// Created by cheilman on 11/11/22.
//

#ifndef BOAF_MAIN_BIRD_H_
#define BOAF_MAIN_BIRD_H_

#include <array>
#include <ostream>
#include <vector>

#include "absl/strings/str_format.h"
#include "raylib.h"
#include "raymath.h"

namespace boaf {

class Bird {
 public:
  explicit Bird(Vector2 pos, int colorIdx);

  bool isSameFlock(const Bird &other) const {
    return this->_colorIdx == other._colorIdx;
  }

  Vector2 getPosition() const { return this->_position; };
  Vector2 getHeading() const { return this->_heading; };
  int getColorIdx() const { return this->_colorIdx; };
  Color getColor() const { return this->_color; };

  /**
   * Must be called from within a BeingDrawing(); EndDrawing(); block.
   */
  void draw() const;

  /**
   * Run away from birds that are too close.
   * @param nearbyBirds
   */
  Vector2 flee(const std::vector<Bird> &nearbyBirds) const;

  /**
   * Group up with our friends.
   * @param nearbyBirds
   */
  Vector2 group(const std::vector<Bird> &nearbyBirds) const;

  /**
   * React based on the set of birds that are nearby.
   */
  void react(const std::vector<Bird> &nearbyBirds);

  /**
   * Move according to our new heading.
   */
  void move(float maxX, float maxY);

  /**
   * Create a random bird with position from 0 to the max value.
   * @param maxX
   * @param maxY
   * @return
   */
  static Bird randomBird(int maxX, int maxY);

 private:
  Vector2 _position;
  Vector2 _heading;
  int _colorIdx;
  Color _color;
};

std::ostream &operator<<(std::ostream &stream, const Vector2 &vec);

std::ostream &operator<<(std::ostream &stream, const Bird &bird);

}  // namespace boaf

#endif  // BOAF_MAIN_BIRD_H_
