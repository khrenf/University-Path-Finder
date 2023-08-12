/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Koby Renfert
// Northwestern University
// CS 211: Winter 2023
// 

#include "building.h"
#include <iostream>



using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

void Building::print(Nodes& nodes) {
  std::cout << this->Name << endl;
    std::cout << "Address: " << this->StreetAddress << endl;
    std::cout << "Building ID: " << this->ID << endl;
    pair mypair = this->getLocation(nodes);
    std::cout << "Approximate location: (" << mypair.first << ", " << mypair.second << ")" << endl;
    std::cout << "Nodes:" << endl;

    for (long long id : this->NodeIDs) {
      double lat, lon;
      bool isEntrance;
      if (nodes.find(id, lat, lon, isEntrance)) {
        std::cout << "  " << id << ": (" << lat << ", " << lon << ")";
        if (isEntrance) {
          cout << ", is entrance" << endl;
        }
        else 
          cout << endl;
      }
      else {
        std::cout << id << ": ** NOT FOUND **" << endl;
      }
    }
}

  // to be called for each node in the footway node vector
  // if the node is = to a node in the building vector, add it to a column for printing later
  bool Building::compareID(long long fID) {
    for (long long bID : this->NodeIDs) { // **** TO DO **** make this loop a function in building (pass through long long fID)
        if (fID == bID) {
            return true;
        }
    }
    return false;
  }


  // finds the center of a building by calculating the average lat/lon of each node defining its perimeter
  pair<double, double> Building::getLocation(Nodes nodes) {
    double avgLat = 0;
    double avgLon = 0;

    for (long long id : this->NodeIDs){
      double lat, lon = 0;
      bool isEntrance;
      nodes.find(id, lat, lon, isEntrance);
      avgLat = avgLat + lat;
      avgLon = avgLon + lon;
    }
    avgLat = avgLat / this->NodeIDs.size();
    avgLon = avgLon / this->NodeIDs.size();

    return make_pair(avgLat, avgLon);
  }


 
