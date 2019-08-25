
#include "PDList.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

PDList::PDList() : numPositions(0) {
  *positions = new PositionDistance[MAX_POS]{};
  for (int i = 0; i < MAX_POS; i++) positions[i] = nullptr;
}

PDList::~PDList() { clear(); }

PDList::PDList(PDList &copy) {
  *positions = new PositionDistance[MAX_POS]{};
  for (int i = 0; i < MAX_POS; i++) positions[i] = nullptr;

  numPositions = copy.size();

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
  // check if position is already in the list
  if (containsCoordinate(position)) {
    // the element to be removed is the last element of the array
    if (indexOf(position) == numPositions - 1) {
      delete positions[numPositions];
      positions[numPositions] = nullptr;
    } else {
      // shift elements in array by 1 position
      for (int i = indexOf(position); i < numPositions; i++)
        positions[i] = positions[i + 1];
    }
    numPositions--;
    // delete last element in array since it's been shifted already
    delete positions[numPositions];

    // initialize deleted element in array
    positions[numPositions] = nullptr;

  }  // if containsCoordinate
  else {
    // add a new position to the list
    positions[numPositions] = position;
    numPositions++;
  }
}

int PDList::indexOf(PDPtr position) {
  for (int i = 0; i < numPositions; i++)
    if (sameCoordinates(position, positions[i])) return i;

  return -1;
}

PDPtr PDList::findPDPtrByCoordinates(int x, int y) {
  for (int i = 0; i < numPositions; i++)
    // if a PDPtr with the same coordinates (x,y) is found in the list
    if (sameCoordinates(x, y, positions[i])) return positions[i];
  return nullptr;
}

bool PDList::sameCoordinates(int x, int y, PDPtr p) {
  return ((p->getX() == x) && (p->getY() == y));
}

bool PDList::sameCoordinates(PDPtr p1, PDPtr p2) {
  return ((p1->getX() == p2->getX()) && (p1->getY() == p2->getY()));
}

bool PDList::containsCoordinate(PDPtr position) {
  for (int i = 0; i < numPositions; i++)
    // if the x and y coordinates of any element in the list
    // matches with position
    if (sameCoordinates(positions[i], position)) return true;

  return false;
}

void PDList::clear() {
  for (int i = 0; i < numPositions; i++) {
    delete positions[i];
    positions[i] = nullptr;
  }
}
