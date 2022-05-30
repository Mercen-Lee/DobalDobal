//
//  main.cpp
//  DodamDodam
//
//  Created by Mercen on 2022/05/25.
//

#include "sha512.h"
#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

int main() {
  
    string id, pw, data;
  
    cout << "아이디: "; cin >> id;
    cout << "비밀번호: "; cin >> pw;
  
    string enc = sha512(pw);
    data = "{\"id\": \"" + id + "\", \"pw\": \"" + enc + "\"}";
    
    CURLcode ret;
    CURL *hnd;
    
    struct curl_slist *slist1;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, "http://auth.dodam.b1nd.com/auth/login");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    ret = curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(slist1);
    slist1 = NULL;
    
    cout << data;
}
