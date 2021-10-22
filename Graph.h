#include <bits/stdc++.h>
using namespace std;

class Node
{
};

class Graph
{
private:
  unordered_map<int, vector<pair<int, int>>> adjacency_list;
  void DFS_Print_Helper1(int s, unordered_map<int, int> &start, unordered_map<int, int> &end);
  void DFS_Print_Helper2(int s, unordered_map<int, bool> &vis, unordered_map<int, int> &start, unordered_map<int, int> &end, ofstream &fout);
  void find_SCC_Helper(int s, vector<int> &disc, vector<int> &low, vector<int> &colour, stack<int> &st);

public:
  int V;
  Graph(int n);
  void add_edge(int v1, int v2, int w);
  void DFS_Print(int s, const char *filename);
  void find_SCC();
};

Graph::Graph(int n = 0)
{
  V = n;
  for (int i = 1; i <= n; ++i)
  {
    adjacency_list[i] = vector<pair<int, int>>();
  }
}

void Graph::add_edge(int v1, int v2, int w)
{
  if (adjacency_list.find(v1) == adjacency_list.end())
    ++V;
  adjacency_list[v1].push_back(make_pair(v2, w));
}

void Graph::DFS_Print(int s, const char *filename)
{
  unordered_map<int, int> start, end;

  for (int i = 1; i <= adjacency_list.size(); ++i)
  {
    if (start.find(i) == start.end())
    {
      // cout << i << endl;
      DFS_Print_Helper1(i, start, end);
    }
  }

  ofstream fout;

  string dot_file = "";
  dot_file = dot_file + filename + ".dot"; // name of graphviz file

  string png_file = "";
  png_file = png_file + filename + ".png"; // name of png file

  fout.open(dot_file.c_str()); // open dot file for writing

  fout << "digraph g {\n";
  fout << "node [shape=record, height=0.1, style=rounded];\n";

  unordered_map<int, bool> vis;

  for (int i = 1; i <= adjacency_list.size(); ++i)
  {
    if (!vis[i])
      DFS_Print_Helper2(i, vis, start, end, fout);
  }

  fout << "}";
  fout.close(); // close dot file

  string str = "dot -Tpng ";
  str = str + dot_file + " -o " + png_file;

  const char *command = str.c_str();

  system(command); // system call to run the dot file using graphviz

  cout << "Graph Printed Successfully! Please check the " << png_file << " file.\n";
}

void Graph::DFS_Print_Helper1(int s, unordered_map<int, int> &start, unordered_map<int, int> &end)
{
  static int time = 0;
  start[s] = ++time;
  for (auto e : adjacency_list[s])
  {
    if (start[e.first] == 0)
    {
      DFS_Print_Helper1(e.first, start, end);
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

void Graph::find_SCC()
{
  vector<int> disc(V + 1), low(V + 1), colour(V + 1);
  stack<int> st;
  cout << "Following are the strongly connected components:\n";
  for (int i = 1; i <= V; ++i)
  {
    if (disc[i] == 0)
      find_SCC_Helper(i, disc, low, colour, st);
  }
}

void Graph::find_SCC_Helper(int s, vector<int> &disc, vector<int> &low, vector<int> &colour, stack<int> &st)
{
  static int time = 0;
  static int component = 0;
  disc[s] = low[s] = ++time;
  // cout << time << endl;
  st.push(s);
  colour[s] = 1;

  for (auto e : adjacency_list[s])
  {
    if (disc[e.first] == 0)
    {
      find_SCC_Helper(e.first, disc, low, colour, st);
      low[s] = min(low[s], low[e.first]);
    }
    else if (colour[e.first] == 1)
    {
      low[s] = min(low[s], disc[e.first]);
    }
  }

  // head node found, pop the stack and print an SCC
  int node = 0; // To store stack extracted vertices
  if (low[s] == disc[s])
  {
    cout << "Component Number " << ++component << ": ";
    while (st.top() != s)
    {
      // cout << st.top() << endl;
      node = st.top();
      cout << node << " ";
      colour[node] = 2;
      st.pop();
    }
    node = st.top();
    cout << node << "\n";
    colour[node] = 2;
    st.pop();
  }
}