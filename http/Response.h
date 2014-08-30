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

#endif /* defined(__match3__NetClientResponse__) */
