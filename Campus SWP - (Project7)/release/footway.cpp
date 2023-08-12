/*footway.cpp*/

//
// A footway / path in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

#include "footway.h"
#include "building.h"
using namespace std;


//
// constructor
//
Footway::Footway(long long id)
  : ID(id)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;

  // vector is default initialized by its constructor
}

//
// adds the given nodeid to the end of the vector
//
void Footway::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

// Runs through each node in the footway's nodeIDs vector, calling Building compareID function on each

void Footway::compareAndVectorize(Building b, vector<long long>& comNodes) {
  for (long long fID : this->NodeIDs) { // **** TO DO **** make it a function in footway
    if (b.compareID(fID)) {
      comNodes.push_back(this->ID);
      break;
    }
  }
  
}
