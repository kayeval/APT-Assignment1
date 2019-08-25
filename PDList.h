
#ifndef COSC_ASS_ONE_POSITION_LIST
#define COSC_ASS_ONE_POSITION_LIST

#include "PositionDistance.h"
#include "Types.h"

#define MAX_POS 100

class PDList {
 public:
  /*                                           */
  /* DO NOT MOFIFY ANY CODE IN THIS SECTION    */
  /*                                           */

  // Create a New Empty List
  PDList();

  // Clean-up the list
  ~PDList();

  // Number of items in the list
  int size();

  // Get a pointer to the position-distance at index i
  PDPtr get(int i);

  // Add a position-distance (as a pointer) to the list
  //    This class now has control over the pointer
  //    And should delete the pointer if the position-distance is removed from
  //    the list
  void addBack(PDPtr position);

  // Checks if the list contains a position-distance with the same co-ordinate
  //    as the given position.
  bool containsCoordinate(PDPtr position);

  // Remove everything from the list
  // Don't forget to clean-up the memory!
  void clear();

  /*                                           */
  /* YOU MAY ADD YOUR MODIFICATIONS HERE       */
  /*                                           */
  PDList(PDList& copy);
  PDPtr findPDPtrByCoordinates(int x, int y);
  bool sameCoordinates(PDPtr p1, PDPtr p2);

  /* This is a suggestion of what you could use. */
  /* You can change this code.                   */
 private:
  PDPtr positions[MAX_POS];  // an array of PositionDistance pointers
  int numPositions;

  bool sameCoordinates(int x, int y, PDPtr p);
  int indexOf(PDPtr position);
};

#endif  // COSC_ASS_ONE_POSITION_LIST
