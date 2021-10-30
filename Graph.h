#include <bits/stdc++.h>
using namespace std;

class Graph
{
private:
  unordered_map<int, vector<pair<int, int>>> adjacency_list;
  void DFS_Print_Helper1(int s, unordered_map<int, int> &start, unordered_map<int, int> &end);
  void DFS_Print_Helper2(int s, unordered_map<int, bool> &vis, unordered_map<int, int> &start, unordered_map<int, int> &end, ofstream &fout);
  void find_SCC_Helper(int s, vector<int> &disc, vector<int> &low, vector<int> &colour, stack<int> &st, vector<vector<int>> &components);
  void DFS_New_Graph(int s, unordered_map<int, bool> &vis, unordered_map<int, vector<pair<int, int>>> &new_adj_list, ofstream &fout);
  void Print_Graph_Helper(int s, unordered_map<int, bool> &vis, ofstream &fout);

public:
  int V;
  Graph(int n);
  void add_edge(int v1, int v2, int w);
  void DFS_Print(int s, const char *filename);
  void find_SCC();
  bool is_semiconnected();
  void Dijkstra(int s);
  void remove_extra_edges();
  void Print_Graph(const char *filename);
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

  vector<vector<int>> components;
  for (int i = 1; i <= V; ++i)
  {
    if (disc[i] == 0)
      find_SCC_Helper(i, disc, low, colour, st, components);
  }

  int no_of_components = components.size();

  for (int i = 0; i < no_of_components; ++i)
  {
    cout << "Component Number " << i + 1 << ": ";
    for (int node : components[i])
      cout << node << " ";
    cout << endl;
  }
}

void Graph::find_SCC_Helper(int s, vector<int> &disc, vector<int> &low, vector<int> &colour, stack<int> &st, vector<vector<int>> &components)
{
  static int time = 0;
  disc[s] = low[s] = ++time;
  st.push(s);
  colour[s] = 1;

  for (auto e : adjacency_list[s])
  {
    if (disc[e.first] == 0)
    {
      find_SCC_Helper(e.first, disc, low, colour, st, components);
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
    vector<int> component;
    while (st.top() != s)
    {
      node = st.top();
      component.push_back(node);
      colour[node] = 2;
      st.pop();
    }
    node = st.top();
    component.push_back(node);
    colour[node] = 2;
    st.pop();

    components.push_back(component);
  }
}

bool Graph::is_semiconnected()
{
  vector<int> roots(V + 1, 0);
  vector<int> root_nodes;
  unordered_map<int, vector<int>> adj_scc;

  vector<int> disc(V + 1), low(V + 1), colour(V + 1);
  stack<int> st;

  vector<vector<int>> components;
  for (int i = 1; i <= V; ++i)
  {
    if (disc[i] == 0)
      find_SCC_Helper(i, disc, low, colour, st, components);
  }

  for (vector<int> component : components)
  {
    int root = component[0];
    for (int node : component)
      roots[node] = root;
    root_nodes.push_back(root);
  }

  vector<int> inDegree(V + 1);

  for (int v = 1; v <= V; ++v)
  {
    for (pair<int, int> e : adjacency_list[v])
    {
      int root_v = roots[v];
      int root_u = roots[e.first];

      if (root_u != root_v)
      {
        adj_scc[root_v].push_back(root_u);
        inDegree[root_u]++;
      }
    }
  }

  int start_node = 0;
  for (int node : root_nodes)
  {
    if (inDegree[node] == 0)
    {
      start_node = node;
      break;
    }
  }

  queue<int> q;
  vector<bool> vis(V + 1);
  q.push(start_node);

  while (!q.empty())
  {
    int node = q.front();
    q.pop();
    vis[node] = true;
    for (int neighbour : adj_scc[node])
    {
      if (vis[neighbour])
        continue;
      q.push(neighbour);
    }
  }

  for (int node : root_nodes)
  {
    if (!vis[node])
      return false;
  }
  return true;
}

void Graph::Dijkstra(int s)
{
  vector<int> distance(V + 1, INT_MAX);
  vector<bool> vis(V + 1, false);
  distance[s] = 0;
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
  q.push(make_pair(0, s));

  while (!q.empty())
  {
    int a = q.top().second;
    q.pop();
    if (vis[a])
      continue;
    vis[a] = true;
    for (pair<int, int> p : adjacency_list[a])
    {
      int b = p.first;
      int w = p.second;
      if (distance[a] + w < distance[b])
      {
        distance[b] = distance[a] + w;
        q.push({distance[b], b});
      }
    }
  }

  cout << "Source: " << s << endl;
  cout << "Node\tDistance\n";
  cout << "=================\n";
  for (int i = 1; i <= V; ++i)
  {
    cout << i << "\t" << distance[i] << endl;
  }
}

void Graph::remove_extra_edges()
{
  vector<int> roots(V + 1, 0);
  vector<int> root_nodes;
  unordered_map<int, vector<int>> adj_scc;

  vector<int> disc(V + 1), low(V + 1), colour(V + 1);
  stack<int> st;

  vector<vector<int>> components;
  for (int i = 1; i <= V; ++i)
  {
    if (disc[i] == 0)
      find_SCC_Helper(i, disc, low, colour, st, components);
  }

  for (vector<int> component : components)
  {
    int root = component[0];
    for (int node : component)
      roots[node] = root;
    root_nodes.push_back(root);
  }

  unordered_map<int, vector<pair<int, int>>> new_adj_list;

  for (pair<int, vector<pair<int, int>>> p : adjacency_list)
  {
    new_adj_list[p.first] = p.second;
  }

  map<pair<int, int>, bool> mp;

  for (int v = 1; v <= V; ++v)
  {
    for (pair<int, int> e : adjacency_list[v])
    {
      int root_v = roots[v];
      int root_u = roots[e.first];

      if (root_v < root_u)
        swap(root_u, root_v);

      if (root_u != root_v)
      {
        // adj_scc[root_v].push_back(root_u);
        if (mp[make_pair(root_u, root_v)])
        {
          // for(auto ee : adjacency_list[v])
          new_adj_list[v].erase(find(new_adj_list[v].begin(), new_adj_list[v].end(), e));
        }
        else
        {
          mp[make_pair(root_u, root_v)] = true;
        }
      }
    }
  }

  const char *filename = "Q3";
  ofstream fout;

  string dot_file = "";
  dot_file = dot_file + filename + ".dot"; // name of graphviz file

  string png_file = "";
  png_file = png_file + filename + ".png"; // name of png file

  fout.open(dot_file.c_str()); // open dot file for writing

  fout << "digraph g {\n";
  fout << "node [style=rounded];\n";

  unordered_map<int, bool> vis;

  for (int i = 1; i <= new_adj_list.size(); ++i)
  {
    if (!vis[i])
      DFS_New_Graph(i, vis, new_adj_list, fout);
  }

  fout << "}";
  fout.close(); // close dot file

  string str = "dot -Tpng ";
  str = str + dot_file + " -o " + png_file;

  const char *command = str.c_str();

  system(command); // system call to run the dot file using graphviz

  cout << "Graph Printed Successfully! Please check the " << png_file << " file.\n";
}

void Graph::DFS_New_Graph(int s, unordered_map<int, bool> &vis, unordered_map<int, vector<pair<int, int>>> &new_adj_list, ofstream &fout)
{
  // fout << s << "\n";
  vis[s] = true;
  for (auto e : new_adj_list[s])
  {
    if (!vis[e.first])
    {
      DFS_New_Graph(e.first, vis, new_adj_list, fout);
    }
    fout << s << " -> " << e.first << "[label=" << e.second << "]\n";
  }
}

void Graph::Print_Graph(const char *filename)
{
  ofstream fout;

  string dot_file = "";
  dot_file = dot_file + filename + ".dot"; // name of graphviz file

  string png_file = "";
  png_file = png_file + filename + ".png"; // name of png file

  fout.open(dot_file.c_str()); // open dot file for writing

  fout << "digraph g {\n";
  fout << "node [style=rounded];\n";

  unordered_map<int, bool> vis;

  for (int i = 1; i <= V; ++i)
  {
    if (!vis[i])
      Print_Graph_Helper(i, vis, fout);
  }

  fout << "}";
  fout.close(); // close dot file

  string str = "dot -Tpng ";
  str = str + dot_file + " -o " + png_file;

  const char *command = str.c_str();

  system(command); // system call to run the dot file using graphviz

  cout << "Graph Printed Successfully! Please check the " << png_file << " file.\n";
}

void Graph::Print_Graph_Helper(int s, unordered_map<int, bool> &vis, ofstream &fout)
{
  vis[s] = true;
  for (pair<int, int> e : adjacency_list[s])
  {
    fout << s << " -> " << e.first << " [label=" << e.second << "]\n";
    if (vis[e.first])
      continue;
    Print_Graph_Helper(e.first, vis, fout);
  }
}