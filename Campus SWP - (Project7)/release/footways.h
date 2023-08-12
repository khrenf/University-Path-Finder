// footways.h

//Collection of footways in the Open Street Map

// Koby Renfert
// Northwestern University
// CS 211: Winter 2023

#pragma once
#include <vector>
#include "tinyxml2.h"
#include "building.h"
#include "footway.h"
using namespace std;
using namespace tinyxml2;

// Keep track of all footways in map



class Footways
{
public:
    vector<Footway> MapFootways;

    // readMapFootways
    // Given XML doc, reads through and stores all in vector

    void readMapFootways(XMLDocument& xmldoc);

    // accessor / getter
    int getNumMapFootways();


    // outputs any footways that share a common node with given building
    void print(Building b);

    // for each footway in MapFootways and creates edges for the graph - to be called in main
    void addEdges(graph& G, Nodes& nodes);

};
