net
===

Simple network protocols wrapper. Current supported protocols: http (via curl)

##Requirements
- C++11
- cURL (current implentation works via libcurl transport for now, but it's easy to implement other transports)

##Simple example

```c++

#include <iostream>
#include "net/http/curl/Curl.h"
#include "net/http/Listener.h"


using namespace poison::net::http;

class HTTPListener : public Listener {
public:

    void onRequestComplete(const Response &resp) override {
        std::cout << "from listener\n";
        std::cout << "got response\ncode: " << resp.code << "\nerror: " << int(resp.error) << "\nerrorText: " << resp.errorText
                << "data: " << resp.data
                << std::endl;

    }

};

int main(int argc, const char * argv[])
{
    Curl http;

    // transport supports listeners
    HTTPListener listener;
    http.addListener(&listener);

    Request::RequestData get;
    Request::RequestData post;

    post["test"] = "test";

    // also you can provide a callback
    Request req("http://google.com", get, post);
    http.send(req, []( const Response& resp ){
        std::cout << "got response\ncode: " << resp.code << "\nerror: " << int(resp.error) << "\nerrorText: " << resp.errorText
                << "data: " << resp.data
                << std::endl;

    });

}


```