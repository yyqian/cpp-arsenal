#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>

using namespace std;

class QueryResult {
friend ostream &operator<<(ostream &out, const QueryResult &res);
public:
  QueryResult(shared_ptr<set<int>> l, shared_ptr<vector<string>> f) : lines(l), file(f) {}
private:
  shared_ptr<set<int>> lines;
  shared_ptr<vector<string>> file;
};

ostream &operator<<(ostream &out, const QueryResult &res) {
  for (int i : *res.lines) {
    out << (*res.file)[i] << endl;
  }
  return out;
}

class TextQuery {
public:
  TextQuery(ifstream &is) : file(new vector<string>) {
    string line;
    while (getline(is, line)) {
      file->push_back(line);
      int n = file->size() - 1;
      istringstream iss(line);
      string word;
      while (iss >> word) {
        if (dict.count(word) == 0) {
          dict[word].reset(new set<int>);
        }
        dict[word]->insert(n);
      }
    }
  }
  QueryResult query(string &s) {
    auto loc = dict.find(s);
    if (loc == dict.end()) {
      return QueryResult(make_shared<set<int>>(), file);
    } else {
      return QueryResult(loc->second, file);
    }
  }
private:
  shared_ptr<vector<string>> file;
  unordered_map<string, shared_ptr<set<int>>> dict;
};

void runQueries(ifstream &infile) {
  TextQuery tq(infile);
  while (true) {
    cout << "Enter word to look for:\n";
    string s;
    getline(cin, s);
    cout << tq.query(s) << endl;
  }
}

int main(int argc, char **argv) {
  ifstream infile("C:/Users/yyqian/OneDrive - patsnap/Concurrency Note.md");
  runQueries(infile);
  return 0;
}