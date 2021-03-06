/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2020, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include "timer.h"
#include "json/json.h"
#include "curlwrapper.h"

/*
curl c++ warpper
https://github.com/jpbarrette/curlpp

c++ promise, future, async
https://modoocode.com/284

simple c++ asynchronous timer
https://gist.github.com/zekroTJA/00317b41aa69f38090071b6c8065272b

쓰레드풀
https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h

json parser
https://github.com/open-source-parsers/jsoncpp
*/

int send_post();
int send_request();

int worker() {
    CURLWrapper cw;
    cw.SendRequest();
    cw.SendPost();
    return 8;
}

int main(void)
{
    std::vector<std::future<int>> request_handle;

    CURLWrapper::Init();

    Timer t1([&](){
        std::cout << "request cnt=" << request_handle.size() << std::endl;
        std::future<int> request = std::async(std::launch::async, worker);  // std::launch::async인자를 사용하여 쓰레드를 바로 생성하고 실행.
        request_handle.push_back(std::move(request));
    }, 1000);
    t1.start();
    std::cout << "input 'q' to quit" << std::endl;
    
    std::string cmd;
    while(1) {
        std::getline(std::cin, cmd);
        if(cmd.compare("q") == 0) break;
    } 
    t1.stop();

    int job_done_cnt = 0;
    for(std::vector<std::future<int>>::iterator itr = request_handle.begin();
    itr != request_handle.end(); itr++) {
        std::future<int> request = std::move(*itr);
        std::future_status status = request.wait_for(std::chrono::microseconds(1));
        if(status == std::future_status::ready) {
            job_done_cnt++;
        }
        else {

        }
    }
    std::cout << job_done_cnt << " job(s) done." << std::endl;

    return 0;
}
