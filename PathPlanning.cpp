/*
 * Kaye Ng's submission for Assignment 1
 *
 *
 * My PathPlanning implementation initializes a maze based on the rows and
 * columns passed into the constructor (allocating space for only the total
 * number of cells in the grid), then stores the initial position and adds it to
 * the list of reachable positions in order to calculate all reachable positions
 * in the grid starting from the initial position.
 *
 * Constructors for each class will set any parameters as it's constructed and
 * destructors for each class will free up any memory it has allocated for each
 * object it owns.
 *
 * For my implementation of getReachablePositions(), I set the initial position
 * as the first cell to be traversed, then I check whether any adjacent position
 * (to the current cell) is an accessible cell, and if so, I add it to the list
 * of reachable positions and set it as the next cell to be traversed while
 * storing a list of visited positions to check whether I've went through
 * all possible reachable positions.
 *
 * For my implementation of getPath(), I set the goal position as the first cell
 * to be traversed and check whether any of its adjacent positions are in the
 * list of reachable positions, then check if it has a distance of exactly
 * one less than the current position (as each position in the shortest path
 * will always contain a distance of one less than the position before it,
 * starting from the goal to the initial position), and if so, I add it to the
 * list of positions containing the shortest path and set it as the next cell to
 * be traversed. I did not make use of a temporary PDList that holds my visited
 * cells, as I only need to compare whether I have reached my initial position
 * in order to exit the loop. Before returning the shortest path list, I call
 * the reverse() function in order to display it in order, from the initial
 * position to the goal position. This reverse function swaps out the first and
 * last memory address being pointed to in each position in the list and
 * continues until the element at position size/2 has been reached.
 *
 * I minimized the allocation of objects on the heap to objects that I would
 * need to access and modify elsewhere in the program from where they were
 * declared, as in the case of PositionDistance objects. If an object allocated
 * on the heap won't be needed anymore, it will be freed up. However, objects
 * that would need to be accessed frequently (such as integers) are allocated on
 * the stack.
 *
 * Each PDList allocates only the space needed for each PDPtr it holds by
 * keeping track of the total number of PDPtr objects in the list.
 *
 * Methods that will only be used in the same class are declared as private
 * methods, to prevent modifications from outside classes.
 *
 * I defined an ADJACENT_SIZE macro to improve software readability.
 *
 * However, some parts in my software design are a bit redundant, specifically
 * in the generation of adjacent positions, as it's used in both
 * getReachablePositions() and getPath().
 *
 */

#include "PathPlanning.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#define ADJACENT_SIZE 4

PathPlanning::PathPlanning(Grid maze, int rows, int cols) {
  this->maze = new char *[rows] {};
  this->rows = rows;
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

    // generate all adjacent cells of current position
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

  // the position to be traversed
  PDPtr currentPos = reachablePositions->findPDPtrByCoordinates(toX, toY);

  if (reachablePositions->containsCoordinate(currentPos)) {
    if (!reachablePositions->sameCoordinates(currentPos, initialPos)) {
      // add the goal position to shortestPath
      shortestPath->addBack(currentPos);
      // while the initial position has not yet been reached
      while (!reachablePositions->sameCoordinates(currentPos, initialPos)) {
        // generate all adjacent cells of current position
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

        i = 0;
        found = false;
        while (!found) {
          // if there's a position in the list with the same coordinates as the
          // current adjacent position being traversed
          if (reachablePositions->containsCoordinate(adjacentCells[i])) {
            // find and assign the position to foundPos
            foundPos = reachablePositions->findPDPtrByCoordinates(
                adjacentCells[i]->getX(), adjacentCells[i]->getY());

            // if the distance of the found position is exactly 1 less than
            // the current position's distance
            if (foundPos->getDistance() == currentPos->getDistance() - 1) {
              found = true;
              // set it as the next position to check
              currentPos = foundPos;

              // add it to shortestPath
              shortestPath->addBack(currentPos);

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
      }
    }
  }

  PDList *copy = new PDList(*shortestPath);
  copy->reverse();
  return copy;
}
