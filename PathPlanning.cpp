/*
 *
 *
 */

// TODO BEFORE SUBMISSION:
// 1 - FINISH WRITING TESTS FOR MILESTONES 2 & 3
// 2 - COMPILE ON SCHOOL'S UNIX SERVER
// 3 - RUN TESTS AND VALGRIND ON COMPILED
// 4 - WRITE REPORT (SEE BELOW)
// 5 - FIX getAdjacentPositions method

// MILESTONES 2 & 3
// At the top of your PathPlanning implementation you must provide a description
// of the design of your implementation. Provide this in a comment-block.
// This block should:
// • Describe (briefly) the approach you have taken in your implementation
// • Describe (briefly) any issues you encountered
//• Justify choices you made in your software design and implementation
// • Analyse (briefly) the efficiency and quality of your implementation,
// identifying both positive and negative aspects of your software

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

// MILESTONE 4
// • Describe (briefly) the approach you have taken in your implementation.
// • Justify (briefly) why your implementation is efficient.

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
  reachablePositions->clear();

  for (int i = 0; i < rows; i++) {
    delete[] maze[i];
    maze[i] = nullptr;
  }
}

void PathPlanning::initialPosition(int x, int y) {
  initialPos = new PositionDistance(x, y, 0);

  // add initial position to the list of reachable positions
  reachablePositions->addBack(initialPos);
}

PDList *PathPlanning::getReachablePositions() {
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
    for (PDPtr pos : adjacentCells) {
      if (!reachablePositions->containsCoordinate(pos)) {
        // if it's an accessible cell
        if (maze[pos->getY()][pos->getX()] == '.')
          reachablePositions->addBack(pos);
      } else
        // free up this position from memory since it won't be needed
        delete pos;
    }

    // add currentPos to list of visited positions
    visited->addBack(currentPos);

    nextToVisit = indexNotVisited(visited);
  } while (nextToVisit >= 0);

  PDList *temp = new PDList(*reachablePositions);
  temp->addBack(initialPos);

  return temp;
}

PDPtr *getAdjacentPositions(PDPtr position) {
  int curX = position->getX();
  int curY = position->getY();
  int distance = position->getDistance() + 1;
  PDPtr *adjacentCells =
      new PDPtr[ADJACENT_SIZE]{// the cell above current position
                               new PositionDistance(curX, curY - 1, distance),

                               // the cell to the left of current position
                               new PositionDistance(curX - 1, curY, distance),

                               // the cell to the right of current position
                               new PositionDistance(curX + 1, curY, distance),

                               // the cell below current position
                               new PositionDistance(curX, curY + 1, distance)};

  return adjacentCells;
}

int PathPlanning::indexNotVisited(PDList *visited) {
  for (int i = 0; i < reachablePositions->size(); i++)
    if (!visited->containsCoordinate(reachablePositions->get(i))) return i;

  return -1;
}

// THIS IS FOR MILESTONE 3 ONLY
//    ONLY IMPLEMENT THIS IF YOU ATTEMPT MILESTONE 3
PDList *PathPlanning::getPath(int toX, int toY) {
  PDList *shortestPath = new PDList();
  bool found;
  int i;
  PDPtr foundPos = nullptr;

  // the position to be visited
  PDPtr currentPos = initialPos;

  // add the initial position to shortestPath
  shortestPath->addBack(currentPos);

  do {
    // generate all adjacent cells of current position

    // TODO: use getAdjacentPositions()
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

    i = 0;
    found = false;
    while (!found) {
      // if there's a position in the list with the same coordinates as the
      // current adjacent position being traversed
      if (reachablePositions->containsCoordinate(adjacentCells[i])) {
        // find and assign the position to foundPos
        foundPos = reachablePositions->findPDPtrByCoordinates(
            adjacentCells[i]->getX(), adjacentCells[i]->getY());

        // if the distance of the found position is exactly 1 more than
        // the current position's distance
        if (foundPos->getDistance() == currentPos->getDistance() + 1) {
          // set it as the next position to check
          currentPos = foundPos;

          // add it to shortestPath
          shortestPath->addBack(currentPos);

          // stop finding for current distance
          found = true;
        } else {
          delete adjacentCells[i];
          adjacentCells[i] = nullptr;
        }
      } else {
        delete adjacentCells[i];
        adjacentCells[i] = nullptr;
      }
      i++;
    }

    while (i < ADJACENT_SIZE) {
      delete adjacentCells[i];
      adjacentCells[i] = nullptr;
      i++;
    }
  }  // while the goal position has not yet been reached
  while (!reachablePositions->sameCoordinates(
      currentPos, reachablePositions->findPDPtrByCoordinates(toX, toY)));

  PDList *copy = new PDList(*shortestPath);
  return copy;
}
