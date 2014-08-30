//
//  NetClientRequest.cpp
//  match3
//
//  Created by Fateev on 19.12.13.
//  Copyright (c) 2013 Game Insight. All rights reserved.
//

#include "Transport.h"
#include "Request.h"

namespace poison { namespace net { namespace http {

    Request::Request(const std::string &_url, const RequestData &_data, const RequestData &_dataPost)
        : url(_url)
        , dataGet(_data)
        , dataPost(_dataPost)
    {

    }

    Request::Request(const std::string& url_, const std::string& binaryData, const RequestData& getData)
        : url(url_)
        , dataGet(getData)
        , binary(binaryData)
    {

    }

    Request::Request(const Request& rvalue)
            : url(rvalue.url)
            , dataGet(rvalue.dataGet)
            , dataPost(rvalue.dataPost)
            , binary(rvalue.binary)
            , headers(rvalue.headers)
    {

    }

    Request::Request(const Request&& rvalue)
            : url(std::move(rvalue.url))
            , dataGet(std::move(rvalue.dataGet))
            , dataPost(std::move(dataPost))
            , binary(std::move(rvalue.binary))
            , headers(std::move(rvalue.headers))
    {

    }

    Request& Request::operator=(const Request&& rvalue) {
        url = std::move(rvalue.url);
        dataGet = std::move(rvalue.dataGet);
        dataPost = std::move(rvalue.dataPost);
        binary = std::move(rvalue.binary);
        return *this;
    }

} } }
