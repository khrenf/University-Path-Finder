/*main.cpp*/

// Koby Renfert
// Northwestern University
// CS 211: Winter 2023

#include <iostream>
#include <string>
#include <iomanip>
#include <utility>
#include <limits>
#include <queue>
#include <algorithm>
#include <stack>
#include "nodes.h"
#include "buildings.h"
#include "footways.h"
#include "building.h"
#include "osm.h"
#include "tinyxml2.h"
#include "node.h"
#include "graph.h"
#include "dist.h"

using namespace std;
using namespace tinyxml2;

// used to verify proper distance calculations upon calling '!' 
void sanityCheck(graph& G);

// used when user prompts to begin navigation function
void navigate(Buildings buildings, Nodes nodes, Footways footways, graph& G);

// helper to be used in navigate
Building findBuilding(Buildings& buildings, Nodes& nodes);

// helper to be used after findBuilding within navigate. Locates close footway and node
// ** OUTPUTS CLOSEST NODE
long long findClosest(Building& building, Footways& footways, Nodes& nodes);

// implementation of dijkstra's algorithm to find the shortest weighted path btwn buildings
vector<long long> dijkstra(graph& G, long long start, long long dest, map<long long, double>& distances);

// helper function within dijkstra to navigate pred array and output nodes
void traversePred(map<long long, long long> predMap, long long startNode, long long endNode);


int main()
{
  cout << setprecision(12);
  Buildings buildings;
  Footways footways;
  XMLDocument xmldoc;
  Nodes nodes;
  graph G;
  
  std::cout << "** NU open street map **" << endl;

  string filename;
  std::cout << endl;
  std::cout << "Enter map filename> " << endl;
  getline(cin, filename);

  //
  // 1. load XML-based map file 
  //
  if (!osmLoadMapFile(filename, xmldoc))
  {
    // failed, error message already output
    return 0;
  }
  
  //
  // 2. read the nodes, which are positions on the map:
  //
  nodes.readMapNodes(xmldoc);
  nodes.sortByID();

  //
  // 3. read the buildings contained in the map:
  buildings.readMapBuildings(xmldoc);

  //
  // 4. read the footways / paths on the map:
  footways.readMapFootways(xmldoc);

  // Build Graph G
  nodes.addVertices(G);
  footways.addEdges(G, nodes);



  //
  // 5. stats
  //
  std::cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
  std::cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;
  std::cout << "# of footways: " << footways.getNumMapFootways() << endl;
  std::cout << "# of graph vertices: " << G.NumVertices() << endl;
  std::cout << "# of graph edges: " << G.NumEdges() << endl;

  //
  // now let the user for search for 1 or more buildings:
  //
  while (true)
  {
    string name;
    std::cout << endl;
    std::cout << "Enter building name, * to list, @ to navigate, or $ to end> " << endl;
    getline(cin, name);  // use getline() because user may type multiple words:

    if (name == "$") {
      break;
    }
    
    else if (name == "*")
    {
      // TODO: list all the buildings in the order they appear in the
      // vector (which is the order they appear in the map file):
      buildings.print();
    }

    else if (name == "!")
    {
      sanityCheck(G);
    }

    else if (name == "@")
    {
      navigate(buildings, nodes, footways, G);
    }

    else 
    {
      buildings.findAndPrint(name, nodes, footways);
    }
      
    
  }//while

  // done:
  std::cout << endl;
  std::cout << "** Done  **" << endl;
  std::cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
  std::cout << "# of Nodes created: " << Node::getCreated() << endl;
  std::cout << "# of Nodes copied: " << Node::getCopied() << endl;
  std::cout << endl;

  return 0;
}

// definition for above declared function. used to verify proper distance calculations upon calling '!' 
void sanityCheck(graph& G) {
  cout << "Graph check of Footway ID 986532630" << endl;

  double w1 = 0;
  double n1 = 9119071425;
  double n2 = 533996671;
  G.getWeight(n1, n2, w1);
  cout << "  Edge: (9119071425, 533996671, " << w1 << ")" << endl;

  double w2 = 0;
  G.getWeight(n2, n1, w2);
  cout << "  Edge: (533996671, 9119071425, " << w2 << ")" << endl;

  double n3 = 533996672;
  double w3 = 0;
  G.getWeight(n2, n3, w3);
  cout << "  Edge: (533996671, 533996672, " << w3 << ")" << endl;

  double w4 = 0;
  G.getWeight(n3, n2, w4);
  cout << "  Edge: (533996672, 533996671, " << w4 << ")" << endl;

  double w5 = 0;
  double n4 = 2240260064;
  G.getWeight(n3, n4, w5);
  cout << "  Edge: (533996672, 2240260064, " << w5 << ")" << endl;

  double w6 = 0;
  G.getWeight(n4, n3, w6);
  cout << "  Edge: (533996672, 2240260064, " << w6 << ")" << endl;
}




// used when user prompts to begin navigation function
void navigate(Buildings buildings, Nodes nodes, Footways footways, graph& G) {

  // find the first building
  cout << "Enter start building name (partial or complete)>" << endl;
  Building startB = findBuilding(buildings, nodes);
  if (startB.ID == 0) {
    cout << "**Start building not found" << endl;
    return;
  }

  // *** find the closest node to the beginning bulding now ***
  long long startNode = findClosest(startB, footways, nodes);

  // now work with the ending building
  cout << "Enter destination building name (partial or complete)>" << endl;
  Building endB = findBuilding(buildings, nodes);
   if (endB.ID == 0) {
    cout << "**Destination building not found" << endl;
    return;
   }

  // *** find the closest node to the ending bulding now ***
  long long endNode = findClosest(endB, footways, nodes);


  // now call dijkstra's to find and return the shortest path
  map<long long, double> distances;
  dijkstra(G, startNode, endNode, distances);
}


// helper to be used in navigate
Building findBuilding(Buildings& buildings, Nodes& nodes) {
  string bname;
  Building startB(0, " ", " ");
  double begLat, begLon = 0;
  std::getline(cin, bname);
  for (Building b : buildings.MapBuildings) { // for each building, compare name with input string
    std::size_t found = b.Name.find(bname);
    if (found != std::string::npos) {
      cout << "  Name: " << b.Name << endl;
      pair<double, double> locationPair = b.getLocation(nodes);
      begLat = locationPair.first;
      begLon = locationPair.second;
      cout << "  Approximate location: (" << begLat << ", " << begLon << ")" << endl;
      startB = b;
      break;
    }
  }
  return startB;
}


// helper to be used after findBuilding within navigate. Locates close footway and node
// ** OUTPUTS CLOSEST NODE
long long findClosest(Building& building, Footways& footways, Nodes& nodes) {
  constexpr double INF = numeric_limits<double>::max();
  double shortestDist = INF;
  long long closeNodeID = 0;
  long long closeFootID = 0;
  pair<double, double> locationPair = building.getLocation(nodes);
  double buildLat = locationPair.first;
  double buildLon = locationPair.second;
  for (Footway f: footways.MapFootways) {
    for (long long nID : f.NodeIDs) {
      double lat, lon = 0;
      bool isEntrance;
      nodes.find(nID, lat , lon, isEntrance);
      if (distBetween2Points(buildLat, buildLon, lat, lon) < shortestDist) {
        shortestDist = distBetween2Points(buildLat, buildLon, lat, lon);
        closeNodeID = nID;
        closeFootID = f.ID;
      }
    }
  }
  cout << "  Closest footway ID " << closeFootID 
  << ", node ID " << closeNodeID
  << ", distance " << shortestDist << endl;
  return closeNodeID;
}

// priotize class to enable priority queue functionality within dijkstra's
class prioritize
{
public:
   bool operator()(const pair<long long, double>& p1, const pair<long long,double>& p2) const
   {
      if (p1.second > p2.second)
         return true;
      else if (p1.second < p2.second)
         return false;
      else
         return p1.first > p2.first;
   }
};

// implementation of dijkstra's algorithm to find the shortest weighted path btwn buildings
vector<long long> dijkstra(graph& G, long long start, long long dest, map<long long, double>& distances) {
  constexpr double INF = numeric_limits<double>::max();
  vector<long long>  visited;
  std::priority_queue<
  pair<long long, double>,
  vector<pair<long long, double>>,
  prioritize> pq_unvisited;
  map<long long, long long> pred; // <node, predeccessor node>
   
  double curr = start;
  for (long long v: G.getVertices()) {
     distances.insert(make_pair(v, INF));
     pred[v] = -1;
     if (v == curr) 
      pq_unvisited.push(make_pair(v, 0.00));
     else
     pq_unvisited.push(make_pair(v, INF));
  }
  distances[curr] = 0;
  while (!pq_unvisited.empty()) {
     pair<double, double> element = pq_unvisited.top();
     pq_unvisited.pop();
     curr = element.first;
     double distance = distances[curr];
     if (distances[curr] == INF)
      break;
     else if (find(visited.begin(), visited.end(), curr) != visited.end())
      continue;
     else
     {
        visited.push_back(curr);
        for (long long adjV : G.neighbors(curr))
        {
           double weight = 0.00;
           G.getWeight(curr, adjV, weight);
           double altPathDistance = distance + weight;
           if (altPathDistance < distances[adjV])
           {
               distances[adjV] = altPathDistance;
               pq_unvisited.push(make_pair(adjV, altPathDistance));
               pred[adjV] = curr;
           }
        }
     }
  }
  cout << "Shortest weighted path:" << endl;
  if (distances[dest] == INF) {
    cout << "**Sorry, destination unreachable" << endl;
    return visited;
  }
  double shortest = INF;
  for (const auto& pair : distances) {
    if ((pair.first) == dest && (pair.second < shortest))
      shortest = pair.second;
  }
  cout << "  # nodes visited: " << visited.size() << endl;
  cout << "  Distance: " << shortest << " miles" << endl;
  traversePred(pred, start, dest);
  return visited;
}

// helper function within dijkstra to navigate pred array and output nodes
void traversePred(map<long long, long long> predMap, long long startNode, long long endNode) {
  constexpr double INF = numeric_limits<double>::max();
  stack<long long> predecessors;
  long long targetNode = endNode; // initialize target to the first node
  while (targetNode != -1) {
    predecessors.push(targetNode);
    targetNode = predMap[targetNode];
  }
  cout << "  Path: ";
  while (!predecessors.empty()) {
    if (predecessors.size() == 1)
      cout << predecessors.top();
    else {
      cout << predecessors.top() << "->";
    }
      predecessors.pop();
  }
  cout << endl;
}




