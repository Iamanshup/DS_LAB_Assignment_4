#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

int main()
{
  Graph *graph = new Graph();
  graph->add_edge(1, 2, 1);
  graph->add_edge(2, 3, 1);
  graph->add_edge(3, 4, 1);
  graph->add_edge(4, 1, 1);
  graph->add_edge(1, 3, 1);
  graph->add_edge(1, 5, 1);

  // cout << LDBL_MIN << endl;
  // cout << sizeof(LDBL_MIN) << endl;

  graph->DFS_Print(1, "test");
}