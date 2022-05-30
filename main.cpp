// //
// //  main.cpp
// //  도발도발 프로젝트
// //
// //  Created by Mercen on 2022/05/25.
// //

#include "sha512.h"
#include "sha512.cpp"

#include "json/json.h"
#include "jsoncpp.cpp"

#include <curl/curl.h>
#include <cstdlib>

#include <iostream>
#include <string>

using namespace std;

void Clear() { cout << "\x1B[2J\x1B[H"; }

size_t curl_callback(void *ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb); return size * nmemb; }

string login() {

    string id, pw, data, enc;
    cout << " * 아이디: "; cin >> id; cout << " * 비밀번호: "; cin >> pw;

    enc = sha512(pw); CURL *curl; CURLcode res;
    data = "{\"id\": \"" + id + "\", \"pw\": \"" + enc + "\"}";

    string response; curl = curl_easy_init(); struct curl_slist *list = NULL;
    curl_easy_setopt(curl, CURLOPT_URL, "http://auth.dodam.b1nd.com:80/auth/login");

    list = curl_slist_append(list, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl); curl_slist_free_all(list);
    curl_easy_cleanup(curl); curl_global_cleanup(); Clear();
    Json::Reader reader; Json::Value root; reader.parse(response, root);

    if(root["status"].asInt()==401) {
        cout << "\n아이디 또는 비밀번호가 잘못되었습니다!\n" << endl; return login(); }

    else if(root["status"].asInt()==200) { cout << "\n로그인에 성공하였습니다! 환영합니다, ";
        cout << root["data"]["member"]["name"].asString() << "님!\n" << endl;
        return root["data"]["token"].asString(); }

    return 0; }

int main(void) { Clear();
    cout << "\n# 도발도발 로그인\n" << endl;
    string token = login(); }