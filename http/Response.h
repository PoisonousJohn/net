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


namespace poison { namespace net { namespace http {
    class Request;

    struct Response {
        enum class Error {
            NoError = 0,
            ConnectionError,
            TimeoutError,
            InternalError, // library error
        };

        const Request* request;

        std::string data;

        /**
        * @brief response code
        */
        int code;

        Error error;

        std::string errorText;

        Response() : code(0), error(Error::NoError) {
        }

        Response(const Response &rvalue) {
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
