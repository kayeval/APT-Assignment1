
#include "PDList.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

PDList::PDList() {
  *positions = new PositionDistance[MAX_POS]{};
  numPositions = 0;

  for (int i = 0; i < MAX_POS; i++) positions[i] = nullptr;
}

PDList::~PDList() { clear(); }

PDList::PDList(PDList &copy) {
  for (int i = 0; i < copy.size(); i++) {
    PDPtr curr = copy.get(i);
    positions[i] =
        new PositionDistance(curr->getX(), curr->getY(), curr->getDistance());
  }
}

int PDList::size() { return numPositions; }

PDPtr PDList::get(int i) {
  if (positions[i]) return positions[i];

  return nullptr;
}

void PDList::addBack(PDPtr position) {
  if (containsCoordinate(
          position)) {  // if coordinates already exist in reachablePositions
    // for (int i = 0; i < numPositions; i++)
    //   std::cout << "positions[" << i << "] = " << "(" << positions[i]->getX()
    //   << "," << positions[i]->getY() << "," << positions[i]->getDistance() <<
    //   ")" << std::endl;

    for (int i = indexToRemove(position); i < numPositions; i++)
      positions[i] = positions[i + 1];  // shift elements in array by 1 position

    delete positions[numPositions - 1];  // delete last element in array since
                                         // it's been shifted already
    positions[numPositions - 1] =
        nullptr;  // initialize deleted element in array
    numPositions--;
  } else {
    positions[numPositions] = position;
    numPositions++;
  }
}

int PDList::indexToRemove(PDPtr position) {
  for (int i = 0; i < numPositions; i++)
    if (containsCoordinate(position)) return i;

  return -1;
}

bool PDList::containsCoordinate(PDPtr position) {
  for (int i = 0; i < numPositions; i++)
    if (positions[i])
      if ((positions[i]->getX() == position->getX()) &&
          (positions[i]->getY() == position->getY()))
        return true;

  return false;
}

void PDList::clear() {
  for (int i = 0; i < numPositions; i++) {
    delete positions[i];
    positions[i] = nullptr;
  }
}
