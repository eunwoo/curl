#pragma once

#include <curl/curl.h>

struct memory {
  char *response;
  size_t size;
};

class CURLWrapper {
public:    
    CURLWrapper();
    ~CURLWrapper();

    static void Init();
    static size_t cb(void *data, size_t size, size_t nmemb, void *userp);

    int SendRequest();
    int SendPost();

private:
    std::string m_data;
};