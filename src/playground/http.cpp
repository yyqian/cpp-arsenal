#include "curl/curl.h"
#include <iostream>

using namespace std;
int main() {
  cout << "Hello" << endl;
  CURL *curl = curl_easy_init();
  CURLcode res;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/inno-data/wopi/monitor/health");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    }
    curl_easy_cleanup(curl);
  }
  return 0;
}