#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <cstring>
#include "json/json.h"

int send_request()
{
  CURL *curl;
  CURLcode res;

  /* get a curl handle */
  curl = curl_easy_init();
  struct memory chunk;
  chunk.response = (char*)malloc(1);
  chunk.size = 0;

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://jsonplaceholder.typicode.com/comments");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
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
      bool ok = jsonReader->parse(chunk.response, chunk.response + chunk.size, &root, &err);
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