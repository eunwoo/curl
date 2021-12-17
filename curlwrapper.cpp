#include <curl/curl.h>
#include <vector>
#include <iostream>
#include "json/json.h"
#include "curlwrapper.h"

CURLWrapper::CURLWrapper() {
    m_data = "";
}

CURLWrapper::~CURLWrapper() {
}

void CURLWrapper::Init() {
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
}

int CURLWrapper::SendRequest()
{
  CURL *curl;
  CURLcode res;

  /* get a curl handle */
  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://jsonplaceholder.typicode.com/comments");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // timeout = 10초

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    else {  // OK
      // std::cout << chunk.response << std::endl;
      std::string err;
      Json::Value root;
      Json::CharReaderBuilder builder;
      Json::CharReader *jsonReader = builder.newCharReader();
      bool ok = jsonReader->parse(m_data.c_str(), m_data.c_str() + m_data.size(), &root, &err);
      if(ok == true) {
        std::cout << "json array size=" << root.size() << std::endl;
        for(Json::Value::ArrayIndex i = 0; i < root.size(); ++i) {
          std::cout << "data no: " << i << std::endl;
          std::cout << "postId: " << root[i]["postId"] << std::endl;
          std::cout << "id: " << root[i]["id"] << std::endl;
          std::cout << "name: " << root[i]["name"] << std::endl;
          std::cout << "email: " << root[i]["email"] << std::endl;
          std::cout << "body: " << root[i]["body"] << std::endl;
        }
      }
      std::cout << "OK" << std::endl;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();    
    return 0;
}

size_t CURLWrapper::cb(void *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  std::string* buf = reinterpret_cast<std::string*>(userp);
  std::vector<char> newData;
  char *cdata = (char*)data;
  newData.assign(cdata, cdata + realsize);
  std::string newStr(newData.begin(), newData.end());
  (*buf) += newStr;
//   std::cout << *buf << std::endl;
  return realsize;
}

int CURLWrapper::SendPost()
{
  CURL *curl;
  CURLcode res;
  struct curl_slist *slist1;
  std::string jsonstr = "{\"nickname\":\"bob\"}";

  slist1 = nullptr;
  slist1 = curl_slist_append(slist1, "Content-Type: application/json");

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://3.34.219.233:9999");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    else {  // OK
      std::cout << "OK" << std::endl;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return 0;    
}