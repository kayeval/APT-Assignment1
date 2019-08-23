
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
  std::cout << "COPY CONSTRUCTOR CALLED" << std::endl;

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
  // check if passed coordinates already exist in the list
  if (containsCoordinate(position)) {
    // shift elements in array by 1 position
    for (int i = indexToRemove(position); i < numPositions; i++)
      positions[i] = positions[i + 1];

    numPositions--;

    // delete last element in array since it's been shifted already
    delete positions[numPositions];

    // initialize deleted element in array
    positions[numPositions] = nullptr;
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

PDPtr PDList::findPDPtr(int x, int y) {
  PDPtr found = nullptr;
  for (int i = 0; i < numPositions; i++)
    if (isEqual(x, y, positions[i])) found = positions[i];
  return found;
}

bool PDList::isEqual(int x, int y, PDPtr p) {
  return ((p->getX() == x) && (p->getY() == y));
}

bool PDList::isEqual(PDPtr p1, PDPtr p2) {
  return ((p1->getX() == p2->getX()) && (p1->getY() == p2->getY()));
}

bool PDList::containsCoordinate(PDPtr position) {
  for (int i = 0; i < numPositions; i++)
    if (positions[i])
      // if the x and y coordinates of any position in the list
      // matches with the passed position
      if (isEqual(positions[i], position)) return true;

  return false;
}

void PDList::clear() {
  for (int i = 0; i < numPositions; i++) {
    delete positions[i];
    positions[i] = nullptr;
  }
}
