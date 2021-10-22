#include <bits/stdc++.h>
using namespace std;

class Node
{
};

class Graph
{
private:
  unordered_map<int, vector<pair<int, int>>> adjacency_list;
  void DFS_Print_Helper1(int s, unordered_map<int, int> &start, unordered_map<int, int> &end, int &time);
  void DFS_Print_Helper2(int s, unordered_map<int, bool> &vis, unordered_map<int, int> &start, unordered_map<int, int> &end, ofstream &fout);

public:
  Graph();
  void add_edge(int v1, int v2, int w);
  void DFS_Print(int s, const char *filename);
};

Graph::Graph()
{
  for (auto u : adjacency_list)
  {
    adjacency_list.erase(u.first);
  }
}

void Graph::add_edge(int v1, int v2, int w)
{
  adjacency_list[v1].push_back(make_pair(v2, w));
}

void Graph::DFS_Print(int s, const char *filename)
{
  unordered_map<int, int> start, end;
  int time = 0;

  DFS_Print_Helper1(s, start, end, time);

  ofstream fout;

  string dot_file = "";
  dot_file = dot_file + filename + ".dot"; // name of graphviz file

  string png_file = "";
  png_file = png_file + filename + ".png"; // name of png file

  fout.open(dot_file.c_str()); // open dot file for writing

  fout << "digraph g {\n";
  fout << "node [shape=record, height=0.1, style=rounded];\n";

  unordered_map<int, bool> vis;
  DFS_Print_Helper2(s, vis, start, end, fout);

  fout << "}";
  fout.close(); // close dot file

  string str = "dot -Tpng ";
  str = str + dot_file + " -o " + png_file;

  const char *command = str.c_str();

  system(command); // system call to run the dot file using graphviz

  cout << "Graph Printed Successfully! Please check the " << png_file << " file.\n";
}

void Graph::DFS_Print_Helper1(int s, unordered_map<int, int> &start, unordered_map<int, int> &end, int &time)
{
  start[s] = ++time;
  for (auto e : adjacency_list[s])
  {
    if (start[e.first] == 0)
    {
      DFS_Print_Helper1(e.first, start, end, time);
    }
  }
  end[s] = ++time;
}

void Graph::DFS_Print_Helper2(int s, unordered_map<int, bool> &vis, unordered_map<int, int> &start, unordered_map<int, int> &end, ofstream &fout)
{
  vis[s] = true;
  fout << s << " [label=\"<f0>|<f1>" << s << "|<f2> " << start[s] << " |<f3> " << end[s] << "\"];\n";
  for (auto e : adjacency_list[s])
  {
    if (!vis[e.first])
    {
      DFS_Print_Helper2(e.first, vis, start, end, fout);
      fout << s << ":f0 -> " << e.first << ":f1 [label=\"Tree Edge\"]\n";
    }
    else
    {
      if (start[s] > start[e.first] && end[s] < end[e.first])
      {
        fout << s << ":f0 -> " << e.first << ":f1 [label=\"Back Edge\" style=\"dashed\"]\n";
      }
      else if (start[s] < start[e.first] && end[s] > end[e.first])
      {
        fout << s << ":f0 -> " << e.first << ":f1 [label=\"Forward Edge\" style=\"dashed\"]\n";
      }
      else if (start[s] > start[e.first] && end[s] > end[e.first])
      {
        fout << s << ":f0 -> " << e.first << ":f1 [label=\"Cross Edge\" style=\"dashed\"]\n";
      }
    }
  }
}