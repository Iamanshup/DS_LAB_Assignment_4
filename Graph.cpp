#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

int main()
{
  int nodes, edges;
  string input_filename;
  int c = 0;

  cout << "Enter file name to import the graph: ";
  cin >> input_filename;

  ifstream file(input_filename);
  file >> nodes >> edges;
  Graph *graph = new Graph(nodes);

  for (int i = 0; i < edges; i++)
  {
    int u, v, w;
    file >> u >> v >> w;
    graph->add_edge(u, v, w);
  }

  file.close();

  while (c != -1)
  {
    int num;
    string filename;
    int start;
    cout << "MENU:\n";
    cout << "1. Print DFS Tree\n2. Print Strongly Connected Components\n3. Graph with Minimum Edges\n4. Is Graph Semi-Connected\n5. Dijkstra\n6. Print Graph\n7. Exit\n";
    cout << "Enter your choice: ";
    cin >> c;
    switch (c)
    {
    case 1:
      try
      {
        cout << "Enter filename(without extension): ";
        cin >> filename;
        graph->DFS_Print(1, filename.c_str());
      }
      catch (...)
      {
        cout << "Some error occurred!\n";
      }
      break;

    case 2:
      graph->find_SCC();
      break;

    case 3:
      try
      {
        cout << "Enter filename(without extension): ";
        cin >> filename;
        graph->remove_extra_edges();
      }
      catch (...)
      {
        cout << "Some error occurred!\n";
      }
      break;

    case 4:
      if (graph->is_semiconnected())
        cout << "The given graph is Semi-Connected.\n";
      else
        cout << "The given graph is not Semi-Connected.\n";
      break;

    case 5:
      cout << "Enter source node for Dijkstra(1 - " << nodes << "): ";
      cin >> start;
      graph->Dijkstra(start);
      break;

    case 6:
      cout << "Enter filename(without extension): ";
      cin >> filename;
      graph->Print_Graph(filename.c_str());
      break;

    case 7:
      c = -1;
      break;

    default:
      cout << "Invalid Choice!\n Try again.\n";
      break;
    }
    cout << "\n\n";
  }

  delete (graph);
}