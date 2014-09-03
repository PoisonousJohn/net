//
//  NetClientResponse.h
//  match3
//
//  Created by Fateev on 19.12.13.
//  Copyright (c) 2013 Game Insight. All rights reserved.
//

#ifndef __match3__NetClientResponse__
#define __match3__NetClientResponse__

#include <iostream>
#include "Request.h"


namespace poison { namespace net { namespace http {

    struct Response {
        enum class Error {
            NoError = 0,
            ConnectionError,
            TimeoutError,
            InternalError, // library error
        };

        Request request;

        std::string data;

        /**
        * @brief response code
        */
        int code;

        Error error;

        std::string errorText;

        Response(const Request& request_)
                : code(0)
                , error(Error::NoError)
                , request(request_)
        {
        }

        Response(const Response &rvalue)
            : request(rvalue.request)
        {
            data = rvalue.data;
            code = rvalue.code;
            error = rvalue.error;
            errorText = rvalue.errorText;
        }

        ~Response() {
        }


    };

} } }

static inline std::ostream& operator<<(std::ostream& stream, const poison::net::http::Response& resp) {
    stream << "Response to url<" << resp.request.getFullUrl() << "> code <" << resp.code << "> data:\n"
    << resp.data << std::endl
    << "post data:\n"
    << poison::net::http::Request::implodeMap( resp.request.getDataPost() )
    << resp.request.getBinaryData()
    << std::endl;
    
    return stream;
}

#endif /* defined(__match3__NetClientResponse__) */
