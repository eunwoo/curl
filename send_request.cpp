#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <cstring>

 struct memory {
   char *response;
   size_t size;
 };
 
 static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
 {
   size_t realsize = size * nmemb;
   struct memory *mem = (struct memory *)userp;
 
   char *ptr = (char*)realloc((void*)mem->response, mem->size + realsize + 1);
   if(ptr == NULL)
     return 0;  /* out of memory! */
 
   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;
 
   return realsize;
 }

int send_request()
{
  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);
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
      std::cout << "OK" << std::endl;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();    
    return 0;
}