// //
// //  main.cpp
// //  DodamDodam
// //
// //  Created by Mercen on 2022/05/25.
// //

#include "sha512.h"
#include "sha512.cpp"

#include "json/json.h"
#include "jsoncpp.cpp"

#include <curl/curl.h>

#include <iostream>
#include <string>

using namespace std;

size_t curl_callback(void *ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb); return size * nmemb; }

string post(string data) {

    CURL *curl; CURLcode res;

    string response; curl = curl_easy_init();
    struct curl_slist *list = NULL;

    curl_easy_setopt(curl, CURLOPT_URL, "http://auth.dodam.b1nd.com:80/auth/login");

    list = curl_slist_append(list, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl); curl_slist_free_all(list);
    curl_easy_cleanup(curl); curl_global_cleanup();
    Json::Reader reader; Json::Value root; reader.parse(response, root);

    return root["data"]["token"].asString();
}

int main(void)
{
    string id, pw, data;
  
    cout << "아이디: "; cin >> id;
    cout << "비밀번호: "; cin >> pw;
  
    string enc = sha512(pw);
    data = "{\"id\": \"" + id + "\", \"pw\": \"" + enc + "\"}";

    cout << post(data) << endl;
}