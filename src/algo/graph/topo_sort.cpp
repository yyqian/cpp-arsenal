#include <deque>
#include <iostream>
#include "graph.h"

void dfs(Graph &graph, int v, vector<bool> &marked, deque<int> &path) {
  marked[v] = true;
  vector<int> adj = graph.getAdj(v);
  for (int e : adj) {
    if (!marked[e]) {
      dfs(graph, e, marked, path);
    }
  }
  path.push_front(v);
}

deque<int> topo_sort(Graph &graph) {
  vector<bool> marked(graph.getV(), false);
  deque<int> path;
  for (int i = 0; i < graph.getV(); ++i) {
    if (!marked[i]) {
      dfs(graph, i, marked, path);
    }
  }
  return path;
}

int main(int argc, char **argv) {
  Graph graph{6};
  graph.addEdge(0, 1);
  graph.addEdge(0, 2);
  graph.addEdge(1, 3);
  graph.addEdge(2, 3);
  graph.addEdge(5, 2);
  graph.addEdge(2, 4);
  deque<int> res = topo_sort(graph);
  for (int v : res) {
    cout << to_string(v);
  }
  cout << endl;
  return 0;
}