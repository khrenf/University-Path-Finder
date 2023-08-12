/*building.h*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

#pragma once

#include <string>
#include <vector>
#include "nodes.h"
#include "node.h"
using namespace std;


//
// Building
//
// Defines a campus building with a name (e.g. "Mudd"), a street
// address (e.g. "2233 Tech Dr"), and the IDs of the nodes that
// define the position / outline of the building.
// 
// NOTE: the Name could be empty "", the HouseNumber could be
// empty, and the Street could be empty. Imperfect data.
//
class Building
{
public:
  long long ID;
  string Name;
  string StreetAddress;
  vector<long long> NodeIDs;

  //
  // constructor
  //
  Building(long long id, string name, string streetAddr);

  //
  // adds the given nodeid to the end of the vector.
  //
  void add(long long nodeid);


  // initially called as part of an iteration through the mapBuilding's building objects:
  // for a given building object, outputs building data,
  // then iterates through each associated node and outputs its data
  void print(Nodes& nodes);


  // to be called for each node in the footway node vector
  // if the node is = to a node in the building vector, returns true
  // to be used later for adding shared footways to a vector for printing
  bool compareID(long long fID);


  // finds the center of a building by calculating the average lat/lon of each node defining its perimeter
  pair<double, double> getLocation(Nodes nodes);

};
