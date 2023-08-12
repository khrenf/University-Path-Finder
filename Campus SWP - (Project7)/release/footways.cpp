// footways.cpp

// Collection of footways in the Open Street Map

// Koby Renfert
// Northwestern University
// CS 211: Winter 2023



#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "osm.h"
#include "tinyxml2.h"
#include "footways.h"
#include "dist.h"

using namespace std;
using namespace tinyxml2;

// Go through file. when encounter footway, create footway object with its info. add node IDS for that footway
// to the object, then add that object to the buildings vector we made before
void Footways::readMapFootways(XMLDocument& xmldoc) {
    XMLElement* osm = xmldoc.FirstChildElement("osm");
    assert(osm != nullptr);
    XMLElement* way = osm->FirstChildElement("way");

    while (way != nullptr) // run until we're done with the file
    {
        if (osmContainsKeyValue(way, "highway", "footway") ||
            osmContainsKeyValue(way, "area:highway", "footway"))  { // if the way is a footway, proceed
            const XMLAttribute* attrId = way->FindAttribute("id");
            long long id = attrId->Int64Value();
        
            Footway F(id); //creates footway object with above value
            XMLElement* nd = way->FirstChildElement("nd");
            while (nd != nullptr) // loop runs so long as we're pointing to something in the list
            { // adds node ID of each node defining the footway to our footway object
                const XMLAttribute* ndref= nd->FindAttribute("ref");
                assert(ndref != nullptr);
                long long id = ndref->Int64Value();
                F.add(id);
                nd = nd->NextSiblingElement("nd");
            }
            this->MapFootways.push_back(F); // once all nodes are added, add the footway object to our vector
        }
    way = way->NextSiblingElement("way"); //advance to next occurence of way and check again for footway status
    }
}

int Footways::getNumMapFootways() { // outputs # of elements in the MapFootways vector, containing all footway objects
    return this->MapFootways.size();
}


// have a building object with nodes in it. For each footway, does each node match each
// prints all commmon intersections of nodes between footways and specified building
void Footways::print(Building b) {
    vector<long long> comNodes; // the common nodes vector will contain footways in which common nodes are shared
    for (Footway f : this->MapFootways) {
        f.compareAndVectorize(b, comNodes);
        // for (long long fID : f.NodeIDs) { // **** TO DO **** make it a function in footway
        //     b.compareID(fID, f, comNodes);
        // }
    //         // for (long long bID : b.NodeIDs) { // **** TO DO **** make this loop a function in building (pass through long long fID)
    //         //     if (fID == bID) {
    //         //         comNodes.push_back(f.ID);
    //         //     }
    //         // }
        }
    // }
    std::sort(comNodes.begin(), comNodes.end());
    std::cout << "Footways that intersect: " << endl;

    if (comNodes.size() == 0) {
        cout << "   None" << endl;
    }
    else {
        long long checkDupe = 0;
        for (long long footID : comNodes) {
            if (footID == checkDupe)
                continue;
            else {
                cout << "   Footway " << footID << endl;
                checkDupe = footID;
            }
        }
    }
}

// for each footway in MapFootways and creates edges for the graph - to be called in main
void Footways::addEdges(graph& G, Nodes& nodes) {
    for (Footway f : this->MapFootways) { // forward loop
        for (size_t i = 0; i < (f.NodeIDs.size() - 1); i++) { 
            long long v1 = f.NodeIDs[i];
            long long v2 = f.NodeIDs[i+1];
            double lat1, lat2, lon1, lon2 = 0.00;
            bool isEntrance1, isEntrance2;
            nodes.find(v1, lat1, lon1, isEntrance1);
            nodes.find(v2, lat2, lon2, isEntrance2);
            double distance = distBetween2Points(lat1, lon1, lat2, lon2);
            G.addEdge(v1, v2, distance);
        }
    }

    for (Footway f : this->MapFootways) { // back loop
        for (size_t i = f.NodeIDs.size() - 1; i > 0; i--) { 
            long long v1 = f.NodeIDs[i];
            long long v2 = f.NodeIDs[i-1];
            double lat1, lat2, lon1, lon2 = 0.00;
            bool isEntrance1, isEntrance2;
            nodes.find(v1, lat1, lon1, isEntrance1);
            nodes.find(v2, lat2, lon2, isEntrance2);
            double distance = distBetween2Points(lat1, lon1, lat2, lon2);
            G.addEdge(v1, v2, distance);
        }
    }
}


