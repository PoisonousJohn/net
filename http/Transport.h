//
//  NetClientService.h
//  match3
//
//  Created by Fateev on 08.11.13.
//  Copyright (c) 2013 Game Insight. All rights reserved.
//

#ifndef __match3__NetClientService__
#define __match3__NetClientService__

#include <iostream>
#include <string>
#include <functional>
#include <set>
#include "Response.h"

namespace poison { namespace net { namespace http {
    class Listener;

    /**
    * @class Transport
    * interface for working with HTTP protocol
    */
    class Transport {
    public:

        typedef std::set<Listener*> Listeners;
        typedef std::function<void(const Response &response)> RequestComplete;

    public:

        Transport() {}

        virtual ~Transport() {}

        /**
        * @brief download a file from url
        */
//        virtual void download(const std::string& url, std::function< void(const std::string& downloadedPath) > onComplete) = 0;
        /**
        * @brief sends request synchronously
        */
        virtual Response send(const Request& request) = 0;
        
        /**
        * @brief send request asynchronously in a separate thread.\n
        * thread will be created for each request.
        */
        virtual void send(const Request& request, RequestComplete onComplete) = 0;

        void addListener(Listener* listener);

        void removeListener(Listener* listener);

        /**
        * @brief you should call update in your event loop
        * @detail update will invoke callbacks and notify listeners
        */
        virtual void update() = 0;

    private:
        // transport should not be copied
        Transport& operator=(const Transport&) = delete;

        Transport(const Transport&) = delete;

        Listeners listeners;

    protected:

        virtual void onRequestComplete(const Response& resp);
    };
    
} } }


#endif /* defined(__match3__NetClientService__) */
