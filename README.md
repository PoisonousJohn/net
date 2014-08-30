net
===

Simple network protocols wrapper. Supported protocols: http (via curl)

## Thread-safety
Not thread-safe.

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

    bool stop = false;
    
    auto respHandler = [&]( const Response& resp ){
        std::cout << "got response\ncode: " << resp.code << "\nerror: " << int(resp.error) << "\nerrorText: " << resp.errorText
                << "data: " << resp.data
                << std::endl;

        stop = true;
    };

    // do request asynchronous (every request will be called on a separate thread)
    // also you can provide callback
    Request req("http://google.com", get, post);
    
    // run synchronously
    auto resp = http.send(req);
    respHandler(resp);
    
    stop = false;
    
    // run asynchronously
    http.send(req, respHandler);

   	// you should call update in your event loop
   	// update will invoke callbacks and notify listeners
    while( !stop ) {
        http.update();
    }

    return 0;

}


```
