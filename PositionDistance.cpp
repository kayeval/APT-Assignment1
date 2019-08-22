
#include "PositionDistance.h"

PositionDistance::PositionDistance() {
  this->x = 0;
  this->y = 0;
  distance = 0;
}

PositionDistance::PositionDistance(int x, int y, int distance) {
  this->x = x;
  this->y = y;
  this->distance = distance;
}

int PositionDistance::getX() { return x; }

int PositionDistance::getY() { return y; }

int PositionDistance::getDistance() { return distance; }