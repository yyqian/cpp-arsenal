#include <iostream>       // std::cout
#include <string>         // std::string
#include <locale>         // std::locale, std::toupper
#include <sstream>

using namespace std;

string toupper(const string &word) {
  locale loc;
  ostringstream oss;
  for (std::string::size_type i = 0; i < word.length(); ++i) {
    oss << std::toupper(word[i], loc);
  }
  return oss.str();
}

int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Usage: java_util CLASSNAME FIELDS" << endl;
    return 1;
  }
  const string classname(argv[1]);
  ostringstream oss;
  oss << "public class " << classname << " {\n";
  for (int i = 2; i != argc; ++i) {
    string ln("public static final String " + toupper(string(argv[i])) + " = \"" + string(argv[i]) + "\";"); // public static final String DESCRIPTION = "description";
    oss << "  " << ln << "\n";
  }
  oss << "}\n";
  cout << oss.str();
  return 0;
}
