#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

int main()
{
  int nodes, edges;
  string filename = "input.txt";
  ifstream file(filename);
  file >> nodes >> edges;
  Graph *graph = new Graph(nodes);

  for (int i = 0; i < edges; i++)
  {
    int u, v, w;
    file >> u >> v >> w;
    graph->add_edge(u, v, w);
  }

  file.close();

  graph->DFS_Print(1, "test");
  // graph->find_SCC();
  // cout << graph->is_semiconnected() << endl;
  // graph->Dijkstra(1);
  graph->remove_extra_edges();
}