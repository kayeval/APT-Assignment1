
#include "PDList.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

PDList::PDList() {
   *positions = new PositionDistance[MAX_POS]{};
   numPositions = 0;

   for (int i = 0; i < MAX_POS; i++)
      positions[i] = nullptr;
}

PDList::~PDList() {
   clear();
}

PDList::PDList(PDList& copy) {
   for (int i = 0; i < copy.size(); i++) {
      PDPtr curr = copy.get(i);
      positions[i] = new PositionDistance(curr->getX(), curr->getY(), curr->getDistance());
   }

   for (int i = 0; i < size(); i++)
         std::cout << "positions[" << i << "] = " << positions[i] << std::endl;
}

int PDList::size() {
   return numPositions;
}

PDPtr PDList::get(int i) {
   if (positions[i])
      return positions[i];

   return nullptr;
}

void PDList::addBack(PDPtr position) {
   // std::cout << "In addBack() &position = " << position << "\tcoor = (" << (*temp)->getX() << "," << (*temp)->getY() << "," << (*temp)->getDistance()
   //         << ")" << std::endl;
   // std::cout << "In addBack() &position = " << position << "\tcoor = (" << position->getX() << "," << position->getY() << "," << position->getDistance()
   //         << ")" << std::endl;

   if (!containsCoordinate(position)) {
      // std::cout << "In if containsCoordinate() &position = " << position << "\tcoor = (" << (*temp)->getX() << "," << (*temp)->getY() << "," << (*temp)->getDistance()
      //      << ")" << std::endl;
      // for(int i = 0; i < size(); i++)
      //    std::cout << "(" << get(i)->getX() << "," << get(i)->getY() << "," << get(i)->getDistance() << ")" << std::endl;
      positions[size()] = position; //new PositionDistance((*temp)->getX(), (*temp)->getY(), (*temp)->getDistance());
      numPositions++;
      // std::cout << "After adding, size of positions[] is: " << size() << std::endl;
      // std::cout << "==============LAST INSERTED ELEMENT " << position << "\tcoor = (" << positions[size() - 1]->getX() << "," << positions[size() - 1]->getY() << "," << positions[size() - 1]->getDistance()
      //      << ")" << std::endl;
   }
   else {
      for (int i = indexToRemove(position); i < size(); i++) //to fix???
         positions[i] = positions[i + 1]; //shift elements in array by 1 position

      delete positions[size()]; //delete last element in array since it's been shifted already
      positions[size() - 1] = nullptr; //initialize deleted element in array
      numPositions--;
   }
}

int PDList::indexToRemove(PDPtr position) {
   for (int i = 0; i < size(); i++)
      if (containsCoordinate(position))
         return i;
   
   return -1;
}

bool PDList::containsCoordinate(PDPtr position) {
   // std::cout << "In containsCoordinate(), position = (" << position->getX() << "," << position->getY() << "," << position->getDistance() << ")" << std::endl;
   // std::cout << "In containsCoordinate(), &position = " << position << std::endl;
   
   for (int i = 0; i < size(); i++)
      if (positions[i])
         if ((positions[i]->getX() == position->getX()) && (positions[i]->getY() == position->getY()))
            return true;

   return false;
}

void PDList::clear() {
   // delete[] positions;

   for (int i = 0; i < size(); i++) {
      delete positions[i];
      positions[i] = nullptr;
   }
}

