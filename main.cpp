// //
// //  main.cpp
// //  도발도발 프로젝트
// //
// //  Created by Mercen on 2022/05/25.
// //


#include "sha512.h"    // 비밀번호 해시를 위한 SHA512 라이브러리
#include "sha512.cpp"  // 출처 : http://www.zedwood.com/article/cpp-sha512-function

#include "json/json.h" // JSON 파싱을 위한 JsonCpp 라이브러리
#include "jsoncpp.cpp" // 출처 : https://github.com/open-source-parsers/jsoncpp

#include <curl/curl.h> // HTTP 통신을 위한 libcurl 라이브러리
#include <termios.h>   // getch() 함수 구현을 위한 termios 라이브러리

#include <iostream>    // C++ 표준 입출력 iostream 라이브러리
#include <string>      // 문자열 제어를 위한 string 라이브러리

using namespace std;   // std 네임스페이스 사용


// 화면 지우기 함수

void Clear() { cout << "\x1B[2J\x1B[H"; } 


// 리눅스용 getch() 함수

int getch() {

    int ch; struct termios legacy; struct termios final;
    tcgetattr(0, &legacy); final = legacy;

    final.c_lflag &= ~(ICANON|ECHO); final.c_cc[VMIN] = 1;
    final.c_cc[VTIME] = 0; tcsetattr(0, TCSAFLUSH, &final);
    ch = getchar(); tcsetattr(0, TCSAFLUSH, &legacy); return ch;

}


// libcurl data 객체 선언

size_t callback(void *ptr, size_t size, size_t nmemb, string* data) {

	data->append((char*)ptr, size * nmemb);
    return size * nmemb;

}


// HTTP POST 통신 함수

string postdata(string url, string data) {

    string response; CURL *curl = curl_easy_init(); // libcurl 세팅

    struct curl_slist *list = NULL; // 헤더 리스트 선언
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // URL 설정

    list = curl_slist_append(list, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list); // 헤더에 data 정보 삽입

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str()); // data 삽입
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // 응답 송신

    curl_easy_perform(curl); curl_slist_free_all(list);
    curl_easy_cleanup(curl); curl_global_cleanup(); // libcurl 정리

    return response; // 응답내용 문자열로 반환

}


// 도담도담 로그인 함수

string login() { 

    string id, pw, data, enc, response; // 문자열 변수 미리 선언

    cout << "  * 아이디: "; cin >> id; // 아이디 입력
    cout << "  * 비밀번호: "; cin >> pw; // 비밀번호 입력

    enc = sha512(pw); // 비밀번호 SHA512로 암호화

    data = "{\"id\": \"" + id + "\", \"pw\": \"" + enc + "\"}"; // JSON 데이터 생성
    response = postdata("http://auth.dodam.b1nd.com:80/auth/login", data); // POST 호출

    Clear(); // 화면 지우기

    Json::Reader reader; Json::Value root; // JSON 변수 미리 선언
    
    reader.parse(response, root); // JSON 파싱 준비
    int status = root["status"].asInt(); // HTTP 응답 코드 불러오기

    if(status==401) { // 로그인 오류(401)시
        cout << "\n# 아이디 또는 비밀번호가 잘못되었습니다!\n" << endl;
        return login(); // 재귀함수 login 호출
    }

    else if(status==200) { // 로그인 성공(200)시
        string name = root["data"]["member"]["name"].asString();

        cout << "\n# 환영합니다, " << name << "님!\n" << endl;
        return root["data"]["token"].asString(); // token 문자열로 반환
    }

    return 0; // 예외 반환 처리

}


// 메인 함수

int main() {
    
    Clear(); // 화면 지우기

    cout << "\n# 도발도발 v0.1 로그인\n" << endl;
    string token = login(); // 도담도담 로그인 후 token 선언
    int command;

    cout << "  - 1. 식단표 확인" << endl;
    cout << "  - 2. 기상송 확인" << endl;
    cout << "  - 3. 자습실 신청" << endl;
    cout << "  - 4. 외출·외박 신청" << endl;
    cout << "\n  * 입력: ";
    cin >> command;
    
}