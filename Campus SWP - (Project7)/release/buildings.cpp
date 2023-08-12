// buildings.cpp

// Collection of buildings in the Open Street Map

// Koby Renfert
// Northwestern University
// CS 211: Winter 2023


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "nodes.h"
#include "osm.h"
#include "buildings.h"
#include "building.h"
#include "footways.h"
#include "tinyxml2.h"


using namespace std;
using namespace tinyxml2;


// Go through file. when encounter building, create building object with its info. add node IDS for that building
// to the object, then add that object to the buildings vector we made before
void Buildings::readMapBuildings(XMLDocument& xmldoc) {
    XMLElement* osm = xmldoc.FirstChildElement("osm");
    assert(osm != nullptr);
    XMLElement* way = osm->FirstChildElement("way");

    while (way != nullptr)
    {
        if (osmContainsKeyValue(way, "building", "university")) {
            const XMLAttribute* attrId = way->FindAttribute("id");
            string name = osmGetKeyValue(way, "name");
            string streetAddr = osmGetKeyValue(way, "addr:housenumber")
                + " "
                + osmGetKeyValue(way, "addr:street");
            long long id = attrId->Int64Value();
        
            Building B(id, name, streetAddr); //creates building object with above values
            XMLElement* nd = way->FirstChildElement("nd");
            while (nd != nullptr) // loop runs so long as we're pointing to something in the list
            { // adds node ID of each node defining the building to our building object
                const XMLAttribute* ndref= nd->FindAttribute("ref");
                assert(ndref != nullptr);
                long long id = ndref->Int64Value();
                B.add(id);
                nd = nd->NextSiblingElement("nd");
            }
            this->MapBuildings.push_back(B); // once all nodes are added, add the building object to our vector
        }
    way = way->NextSiblingElement("way"); //advance to next occurence of way and check again for building status
    }
}

    



int Buildings::getNumMapBuildings(){ // outputs # of elements in the MapBuildings vector, holding all buildings objects
    // MapBuildings
    return this->MapBuildings.size();
}

// outputs all buildings in the mapBuildings vector and associated data
void Buildings::print() {
    for (Building b : this->MapBuildings) {
        std::cout << b.ID << ": " << b.Name << ", " << b.StreetAddress << endl;
    }
}


// When called in Main, searches for specified building and outputs associated data
void Buildings::findAndPrint(string name, Nodes nodes, Footways footways) {
    for (Building b : this->MapBuildings) {
        std::size_t found = b.Name.find(name);
        if (found != std::string::npos) {
          b.print(nodes);
          footways.print(b);
        }
      }
}




