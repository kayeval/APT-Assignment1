
#include "PathPlanning.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

PathPlanning::PathPlanning(Grid maze, int rows, int cols) {
   this->maze = new char*[rows]{};
   this->rows = rows;
   this->cols = cols;
   this->initialPos = nullptr;

   for (int i = 0; i < rows; i++)
      this->maze[i] = new char[cols]{};

   for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
         this->maze[i][j] = maze[i][j];

   //initialize reachablePositions
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

PDList* PathPlanning::getReachablePositions() {
   /*
   ================================PSEUDOCODE:================================
   Let x be the starting position of the robot with distance 0
   Let P be a list of positions the robot can reach, with distances (initially contains x)
   Let T be a temporary list (initially empty)

   repeat
   | Select an item p from P that is not in T
   | foreach Position, q that the robot can reach from p do
      | | Set the distance of q to be one more than the distance of p
      | | Add q to P if and only if there is no item in P with the same co-ordinate as q
   | end
   | Add p to T
   until No such position p can be found
   */

  PDList* visited = new PDList();
  int i = 0;
  PDPtr currentPos = reachablePositions->get(0);

  do {
     currentPos = reachablePositions->get(i);
   //   std::cout<< "========= currentPos: (" << currentPos->getX() << "," << currentPos->getY() << ")" << std::endl;
     
     int curX = currentPos->getX();
     int curY = currentPos->getY();
     int distance = currentPos->getDistance() + 1;
     
     //note: row col is stored as (col, row) in coordinates
     PositionDistance* adjacentCells[4] = {
        new PositionDistance(curX, curY - 1, distance),
        new PositionDistance(curX - 1, curY, distance),
        new PositionDistance(curX + 1, curY, distance),
        new PositionDistance(curX, curY + 1, distance)
        };

     for (PositionDistance* pos : adjacentCells)
        if (maze[pos->getY()][pos->getX()] == '.')
           reachablePositions->addBack(pos);
     
     for (int j = 0; j < reachablePositions->size(); j++)
      std::cout << "reachablePositions.get(" << j << ") = " << reachablePositions->get(j) << std::endl;
     
     visited->addBack(currentPos); //add currentPos to visited list

      for (int j = 0; j < visited->size(); j++)
      std::cout << "\nvisited.get(" << j << ") = " << visited->get(j) << std::endl;
     
     i++;
  } while (!visited->containsCoordinate(currentPos)); //TO FIX: ONLY LOOPING ONCE

  PDList* temp = reachablePositions;
  temp->addBack(initialPos);
  return temp;
}

// THIS IS FOR MILESTONE 3 ONLY
//    ONLY IMPLEMENT THIS IF YOU ATTEMPT MILESTONE 3
PDList* PathPlanning::getPath(int toX, int toY) {


   return nullptr;
}
