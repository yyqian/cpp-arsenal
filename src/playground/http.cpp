#include <curl/curl.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

//string host("http://localhost:8080");
string host("http://api-gateway-ci5.innosnap.local");
string app_key("-H 'X-App-Key: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiI1MzZlMjBlNjVhMzQ0OGU1YjczNTY3OGNhNjQ5NGJjYiIsImlhdCI6MTQ5ODEwMDQyNCwiZXhwIjo0NjUxNzAwNDI0LCJjbGllbnRfdHlwZSI6IjMiLCJhcHBfa2V5IjoiNmY1ODk5YjVkMmUwMTE3Mzg4MjVhYzBmZjIxNjU0YWQiLCJleHRyYSI6W119.-Ce6qQf_IKGppXKdmCXvYIIN0oyZusFXB_K86tYPptY'");

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

void next_job() {
  string cmd = "curl " + app_key + " -H 'X-Consumer-Tenant-ID: 10' -H 'X-Consumer-User-ID: 1' " + host + "/inno-data/wopi/archive/patents/upload-next";
  system(cmd.c_str());
}

void submit_job() {
  string cmd = "curl " + app_key + " -F 'file=@/Users/yyqian/Downloads/CN_105073917_A.pdf' -X POST -H 'X-Consumer-Tenant-ID: 10' -H 'X-Consumer-User-ID: 1' " + host + "/inno-data/wopi/archive/patents/upload";
  system(cmd.c_str());
}

string get_job() {
  string response_string;
  CURL *curl = curl_easy_init();
  if (curl) {
    string url = host + "/inno-data/wopi/archive/patents/upload-result";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "X-Consumer-Tenant-ID: 10");
    headers = curl_slist_append(headers, "X-Consumer-User-ID: 1");
    headers = curl_slist_append(headers, "X-App-Key: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiI1MzZlMjBlNjVhMzQ0OGU1YjczNTY3OGNhNjQ5NGJjYiIsImlhdCI6MTQ5ODEwMDQyNCwiZXhwIjo0NjUxNzAwNDI0LCJjbGllbnRfdHlwZSI6IjMiLCJhcHBfa2V5IjoiNmY1ODk5YjVkMmUwMTE3Mzg4MjVhYzBmZjIxNjU0YWQiLCJleHRyYSI6W119.-Ce6qQf_IKGppXKdmCXvYIIN0oyZusFXB_K86tYPptY");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
    CURLcode res = curl_easy_perform(curl);
    //cout << "Header: " << endl;
    //cout << header_string << endl;
    //cout << "Body: " <<endl;
    //cout << response_string << endl;
    curl_easy_cleanup(curl);
  }
  return response_string;
}

void cancel_job() {
  CURL *curl = curl_easy_init();
  if (curl) {
    string url = host + "/inno-data/wopi/archive/patents/upload";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // redirect
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "X-Consumer-Tenant-ID: 10");
    headers = curl_slist_append(headers, "X-Consumer-User-ID: 1");
    headers = curl_slist_append(headers, "X-App-Key: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiI1MzZlMjBlNjVhMzQ0OGU1YjczNTY3OGNhNjQ5NGJjYiIsImlhdCI6MTQ5ODEwMDQyNCwiZXhwIjo0NjUxNzAwNDI0LCJjbGllbnRfdHlwZSI6IjMiLCJhcHBfa2V5IjoiNmY1ODk5YjVkMmUwMTE3Mzg4MjVhYzBmZjIxNjU0YWQiLCJleHRyYSI6W119.-Ce6qQf_IKGppXKdmCXvYIIN0oyZusFXB_K86tYPptY");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    string response_string;
    string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
    CURLcode res = curl_easy_perform(curl);
    //cout << "Header: " << endl;
    //cout << header_string << endl;
    //cout << "Body: " <<endl;
    //cout << response_string << endl;
    curl_easy_cleanup(curl);
  }
}

int main() {
  for (int i = 1; i < 100; ++i) {
    next_job();
    submit_job();
    usleep(i * 100 * 1000);
    cancel_job();
    string res = get_job();
    if (res.find("EMPTY") == std::string::npos) {
      cout << to_string(i) << endl;
      cout << res << endl;
    }
  }
  return 0;
}