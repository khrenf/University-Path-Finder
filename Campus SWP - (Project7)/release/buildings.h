// buildings.h

//Collection of buildings in the Open Street Map

// Koby Renfert
// Northwestern University
// CS 211: Winter 2023

#pragma once
#include <vector>
#include "tinyxml2.h"
#include "footways.h"
using namespace std;
using namespace tinyxml2;

// Keep track of all buildlings in map

class Buildings
{
public:
    vector<Building> MapBuildings;
    // readMapBuildings
    // Given XML doc, reads through and stores all in vector

    void readMapBuildings(XMLDocument& xmldoc);

    // accessor / getter
    int getNumMapBuildings();

    // print function be called in main. iterates through each
    // building in mapBuildings and outputs associated data
    void print();


    // When called in Main, searches for specified building and outputs associated data
    void findAndPrint(string name, Nodes nodes, Footways footways); 

};