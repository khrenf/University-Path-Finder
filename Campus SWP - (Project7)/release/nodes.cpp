/*nodes.cpp*/

//
// A collection of nodes in the Open Street Map.
// 
// Professor Joe Hummel
// Koby Renfert
// Northwestern University
// CS 211: Winter 2023
// 
// References:
// 
// TinyXML: 
//   files: https://github.com/leethomason/tinyxml2
//   docs:  http://leethomason.github.io/tinyxml2/
// 
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <map>
#include <cassert>
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;


//
// readMapNodes
//
// Given an XML document, reads through the document and 
// stores all the nodes into the given vector. Each node
// is a point on the map, with a unique id along with 
// (lat, lon) position. Some nodes are entrances to buildings,
// which we capture as well.
//
void Nodes::readMapNodes(XMLDocument& xmldoc)
{
  XMLElement* osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document node by node: 
  //
  XMLElement* node = osm->FirstChildElement("node");

  while (node != nullptr)
  {
    const XMLAttribute* attrId = node->FindAttribute("id");
    const XMLAttribute* attrLat = node->FindAttribute("lat");
    const XMLAttribute* attrLon = node->FindAttribute("lon");

    assert(attrId != nullptr);
    assert(attrLat != nullptr);
    assert(attrLon != nullptr);

    long long id = attrId->Int64Value();
    double latitude = attrLat->DoubleValue();
    double longitude = attrLon->DoubleValue();

    //
    // is this node an entrance? Check for a 
    // standard entrance, the main entrance, or
    // one-way entrance.
    //
    bool entrance = false;

    if (osmContainsKeyValue(node, "entrance", "yes") ||
      osmContainsKeyValue(node, "entrance", "main") ||
      osmContainsKeyValue(node, "entrance", "entrance"))
    {
      entrance = true;
    }

    //
    // Add node to vector:
    // 
    // This creates an object then pushes copy into vector:
    //
    //   Node N(id, latitude, longitude, entrance);
    //   this->MapNodes.push_back(N);
    //
    // This creates just one object "emplace", avoiding 
    // an extra Node copy each time:
    //
    this->MapNodes.emplace(id, Node(id, latitude, longitude, entrance));
    //
    // next node element in the XML doc:
    //
    node = node->NextSiblingElement("node");
  }
}

//
// sortByID
//
// sorts the nodes into ascending order by ID.
//
void Nodes::sortByID()
{

  // sort(this->MapNodes.begin(), this->MapNodes.end(), [](Node& n1, Node& n2) 
  // {
  //   if (n1.getID() < n2.getID())
  //     return true;
  //   else
  //     return false;
  // });
}
  //
  // TODO: use built-in sort algorithm + lambda expression
  //



//
// find
// 
// Searches the nodes for the one with the matching ID, returning
// true if found and false if not. If found, a copy of the node 
// is returned via the node parameter, which is passed by reference.
//
bool Nodes::find(long long id, double& lat, double& lon, bool& isEntrance)
{
  auto ptr = this->MapNodes.find(id);

  if (ptr == this->MapNodes.end()) { // not found
    return false;
  }

  else { // found
    lat = this->MapNodes.at(id).getLat();
    lon = this->MapNodes.at(id).getLon();
    isEntrance = this->MapNodes.at(id).getIsEntrance();
    return true;
  }
}

//
// accessors / getters
//
int Nodes::getNumMapNodes() {
  return (int) this->MapNodes.size();
}

// to be called within main, inputs all nodes as vertices
void Nodes::addVertices(graph& G) {
  for (const auto& [ID, node] : this->MapNodes) {
    G.addVertex(ID);
  }
}
