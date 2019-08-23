
#include "PositionDistance.h"

PositionDistance::PositionDistance() : x(0), y(0), distance(0) {}

PositionDistance::PositionDistance(int x, int y, int distance)
    : x(x), y(y), distance(distance) {}

PositionDistance::PositionDistance(PositionDistance& copy)
    : x(copy.getX()), y(copy.getY()), distance(copy.getDistance()) {}

int PositionDistance::getX() { return x; }

int PositionDistance::getY() { return y; }

int PositionDistance::getDistance() { return distance; }
