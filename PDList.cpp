
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
    positions[i] = new PositionDistance(*copy.get(i));
  }
}

int PDList::size() { return numPositions; }

PDPtr PDList::get(int i) {
  if (positions[i]) return positions[i];

  return nullptr;
}

void PDList::addBack(PDPtr position) {
  // check if passed coordinates already exist in the list
  if (containsCoordinate(position)) {
    // shift elements in array by 1 position
    for (int i = indexToRemove(position); i < numPositions; i++)
      positions[i] = positions[i + 1];

    // delete last element in array since it's been shifted already
    delete positions[numPositions - 1];

    // initialize deleted element in array
    positions[numPositions - 1] = nullptr;
    numPositions--;
  } else {
    // add a new position to the list
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
      // if the x and y coordinates of any position in the list
      // matches with the passed position
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
