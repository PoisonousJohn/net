#ifndef __curl_H_
#define __curl_H_

#include <mutex>
#include <curl/curl.h>
#include "Transport.h"

namespace poison { namespace net { namespace http {

    class Curl : public Transport {

    public:


        virtual Response send(const Request &request) override;

        void send(const Request& request, RequestComplete onComplete) override;

    protected:


        void CurlCheckError( CURLcode code, const char* errbuff );

        /**
        * @brief override this to set custom curl options
        */
        virtual void setCurlOptions(CURL* curl) {}

    };

} } }


#endif //__curl_H_
