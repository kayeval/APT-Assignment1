
// TODO BEFORE SUBMISSION:

// 1 - COMPILE ON SCHOOL'S UNIX SERVER
// 2 - RUN TESTS AND VALGRIND ON COMPILED

// 2 - ADD DOCUMENTATION
// At the top of your PathPlanning implementation you must provide a description
// of the design of your implementation. Provide this in a comment-block.
// This block should:
// • Describe (briefly) the approach you have taken in your
// implementation
// • Describe (briefly) any issues you encountered •
// Justify choices you made in your software design and implementation
// • Analyse (briefly) the efficiency and quality of your implementation,
// identifying both positive and negative aspects of your software

#include "PathPlanning.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#define ADJACENT_SIZE 4

PathPlanning::PathPlanning(Grid maze, int rows, int cols) {
  this->maze = new char *[rows] {};
  this->rows = rows;
  this->cols = cols;
  this->initialPos = nullptr;

  for (int i = 0; i < rows; i++) this->maze[i] = new char[cols]{};

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) this->maze[i][j] = maze[i][j];

  reachablePositions = new PDList();
}

PathPlanning::~PathPlanning() {
  reachablePositions->clear();  // TO RESOLVE: invalid read?

  for (int i = 0; i < rows; i++) {
    delete[] maze[i];
    maze[i] = nullptr;
  }
}

void PathPlanning::initialPosition(int x, int y) {
  initialPos = new PositionDistance(x, y, 0);
  reachablePositions->addBack(initialPos);
}

PDList *PathPlanning::getReachablePositions() {
  /*
  ================================PSEUDOCODE:================================
  Let x be the starting position of the robot with distance 0
  Let P be a list of positions the robot can reach, with distances (initially
  contains x) Let T be a temporary list (initially empty)

  repeat
  | Select an item p from P that is not in T
  | foreach Position, q that the robot can reach from p do
     | | Set the distance of q to be one more than the distance of p
     | | Add q to P if and only if there is no item in P with the same
  co-ordinate as q | end | Add p to T until No such position p can be found
  */

  PDList *visited = new PDList();
  int nextToVisit = 0;
  PDPtr currentPos = nullptr;

  do {
    currentPos = reachablePositions->get(nextToVisit);

    int curX = currentPos->getX();
    int curY = currentPos->getY();
    int distance = currentPos->getDistance() + 1;

    PDPtr adjacentCells[ADJACENT_SIZE] = {
        // the cell above current position
        new PositionDistance(curX, curY - 1, distance),

        // the cell to the left of current position
        new PositionDistance(curX - 1, curY, distance),

        // the cell to the right of current position
        new PositionDistance(curX + 1, curY, distance),

        // the cell below current position
        new PositionDistance(curX, curY + 1, distance)};

    // check if there are any accessible adjacent cells
    // (excluding the initial position)
    for (PDPtr pos : adjacentCells) {
      if (!reachablePositions->containsCoordinate(pos)) {
        if (maze[pos->getY()][pos->getX()] == '.')
          reachablePositions->addBack(pos);
      } else
        // free up this position from memory since it won't be needed
        delete pos;
    }

    // add currentPos to visited list
    visited->addBack(currentPos);

    nextToVisit = indexNotVisited(visited);
  } while (nextToVisit >= 0);

  PDList *temp = new PDList(*reachablePositions);
  temp->addBack(initialPos);

  return temp;
}

// TO FIX
PDPtr *getAdjacentPositions(PDPtr position) {
  // int curX = position->getX();
  // int curY = position->getY();
  // int distance = position->getDistance() + 1;

  // is an array with automatic storage duration. Memory where it resides is
  // deallocated once the execution goes out of the scope of your function, thus
  // pointer to this memory that you return becomes invalid (dangling pointer).
  // Trying to access memory, that has already been deallocated,
  //     results in undefined behaviour.

  // PDPtr adjacentCells[ADJACENT_SIZE] = {
  //     // the cell above current position
  //     new PositionDistance(curX, curY - 1, distance),

  //     // the cell to the left of current position
  //     new PositionDistance(curX - 1, curY, distance),

  //     // the cell to the right of current position
  //     new PositionDistance(curX + 1, curY, distance),

  //     // the cell below current position
  //     new PositionDistance(curX, curY + 1, distance)};

  // return adjacentCells;
  return nullptr;
}

int PathPlanning::indexNotVisited(PDList *visited) {
  for (int i = 0; i < reachablePositions->size(); i++)
    if (!visited->containsCoordinate(reachablePositions->get(i))) return i;

  return -1;
}

// THIS IS FOR MILESTONE 3 ONLY
//    ONLY IMPLEMENT THIS IF YOU ATTEMPT MILESTONE 3
PDList *PathPlanning::getPath(int toX, int toY) {
  PDList *shortestPath = new PDList(*reachablePositions);

  // for (int i = 0; i < shortestPath->size(); i++)
  //   std::cout << "shortestPath[" << i << "] = (" <<
  //   shortestPath->get(i)->getX()
  //             << "," << shortestPath->get(i)->getY() << ","
  //             << shortestPath->get(i)->getDistance() << ")" << std::endl;

  PDPtr currentPos = shortestPath->findPDPtrByCoordinates(toX, toY);
  PDPtr foundPos = nullptr;

  do {
    // generate all adjacent cells of current position

    // TODO: use getAdjacentPositions()
    int curX = currentPos->getX();
    int curY = currentPos->getY();
    int distance = currentPos->getDistance() - 1;

    PDPtr adjacentCells[ADJACENT_SIZE] = {
        // the cell above current position
        new PositionDistance(curX, curY - 1, distance),

        // the cell to the left of current position
        new PositionDistance(curX - 1, curY, distance),

        // the cell to the right of current position
        new PositionDistance(curX + 1, curY, distance),

        // the cell below current position
        new PositionDistance(curX, curY + 1, distance)};

    for (int j = 0; j < ADJACENT_SIZE; j++) {
      // check if any adjacent cells are already in shortestPath
      if (shortestPath->containsCoordinate(adjacentCells[j])) {
        foundPos = shortestPath->findPDPtrByCoordinates(
            adjacentCells[j]->getX(), adjacentCells[j]->getY());

        // if it's not the initial or goal coordinate
        if (!(shortestPath->sameCoordinates(initialPos, foundPos) ||
              shortestPath->sameCoordinates(
                  shortestPath->findPDPtrByCoordinates(toX, toY), foundPos))) {
          // if there's a position in the list with the same distance as
          // generated adjacent cell
          if (foundPos->getDistance() == distance) {
            currentPos = foundPos;
            // remove all other positions with the same distance
            shortestPath->removePDPtrWithSameDistance(currentPos);
          } else {
            // if it has a distance
            // less than what's in the list, remove it from the list
            if (adjacentCells[j]->getDistance() < foundPos->getDistance()) {
              shortestPath->addBack(adjacentCells[j]);
            }
            delete adjacentCells[j];
          }  // if not initial or goal pos
        }    // if containsCoordinate
        else
          delete adjacentCells[j];
      }

      // if distance of adjacent cell == prev loc - 1, keep it in shortestPath
      // and set it as prev
      // else remove all other adjacent cells from shortestPath

      // select a PDPtr with a distance of currentPos - 1 from the list
      // currentPos = shortestPath->findPDPtrByDistance(distance);
    }
  }
  // while distance is more than 0
  while (currentPos->getDistance() > 0);

  PDList *copy = new PDList(*shortestPath);
  return copy;
}
