#include <iostream>
#include <vector>
using namespace std;

class Graph {
  int V;  // 顶点数目
  vector<int> *adj;

 public:
  Graph(int v) : V(v), adj(new vector<int>[v]) {}
  ~Graph() { delete[] adj; }
  void addEdge(int v, int w) { adj[v].push_back(w); }
  int getV() { return V; }
  vector<int> &getAdj(int v) { return adj[v]; }

  void print() {
    for (int i = 0; i < V; i++) {
      for (int ele : adj[i]) {
        cout << to_string(i) << "->" << to_string(ele) << endl;
      }
    }
  }
};