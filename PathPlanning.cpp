// TODO
// At the top of your PathPlanning implementation you must provide a description
// of the design of your implementation. Provide this in a comment-block. This
// block should: • Describe (briefly) the approach you have taken in your
// implementation • Describe (briefly) any issues you encountered • Justify
// choices you made in your software design and implementation • Analyse
// (briefly) the efficiency and quality of your implementation, identifying both
// positive and negative aspects of your software

#include "PathPlanning.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

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
  delete reachablePositions;

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
  PDPtr currentPos;

  do {
    currentPos = reachablePositions->get(nextToVisit);

    int curX = currentPos->getX();
    int curY = currentPos->getY();
    int distance = currentPos->getDistance() + 1;

    PositionDistance *adjacentCells[4] = {
        new PositionDistance(curX, curY - 1,
                             distance),  // cell above current position
        new PositionDistance(curX - 1, curY,
                             distance),  // cell to the left of current position
        new PositionDistance(
            curX + 1, curY,
            distance),  // cell to the right of current position
        new PositionDistance(curX, curY + 1,
                             distance)  // cell below current position
    };

    // check adjacent cells excluding the initial position
    for (PositionDistance *pos : adjacentCells) {
      if (!reachablePositions->containsCoordinate(pos)) {
        if (maze[pos->getY()][pos->getX()] == '.')
          reachablePositions->addBack(pos);
      } else
        delete pos;
    }

    visited->addBack(currentPos);  // add currentPos to visited list
    nextToVisit = indexNotVisited(visited);
  } while (nextToVisit >= 0);

  PDList *temp = reachablePositions;
  temp->addBack(initialPos);
  return temp;
}

int PathPlanning::indexNotVisited(PDList *visited) {
  for (int i = 0; i < reachablePositions->size(); i++)
    if (!visited->containsCoordinate(reachablePositions->get(i))) return i;

  return -1;
}

// TODO: COMPILE ON SCHOOL'S UNIX SERVER

// THIS IS FOR MILESTONE 3 ONLY
//    ONLY IMPLEMENT THIS IF YOU ATTEMPT MILESTONE 3
PDList *PathPlanning::getPath(int toX, int toY) { return nullptr; }
