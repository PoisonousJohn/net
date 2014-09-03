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
        , post (false)
    {
        if (!dataPost.empty()) {
            post = true;
        }

    }

    Request::Request(const std::string& url_, const std::string& binaryData, const RequestData& getData)
        : url(url_)
        , dataGet(getData)
        , binary(binaryData)
        , post(true)
    {

    }

    Request::Request(const Request& rvalue)
            : url(rvalue.url)
            , dataGet(rvalue.dataGet)
            , dataPost(rvalue.dataPost)
            , binary(rvalue.binary)
            , headers(rvalue.headers)
            , queryString(rvalue.queryString)
            , post(rvalue.post)
    {

    }

    Request::Request(const Request&& rvalue)
            : url(std::move(rvalue.url))
            , dataGet(std::move(rvalue.dataGet))
            , dataPost(std::move(rvalue.dataPost))
            , binary(std::move(rvalue.binary))
            , headers(std::move(rvalue.headers))
            , queryString(std::move(rvalue.queryString))
            , post(rvalue.post)
    {

    }

    Request& Request::operator=(const Request&& rvalue) {
        url = std::move(rvalue.url);
        dataGet = std::move(rvalue.dataGet);
        dataPost = std::move(rvalue.dataPost);
        binary = std::move(rvalue.binary);
        headers = std::move(rvalue.headers);
        queryString = std::move(rvalue.queryString);
        post = rvalue.post;
        
        return *this;
    }

    Request& Request::operator=(const Request& rvalue) {
        url = rvalue.url;
        dataGet = rvalue.dataGet;
        dataPost = rvalue.dataPost;
        binary = rvalue.binary;
        headers = rvalue.headers;
        queryString = rvalue.queryString;
        post = rvalue.post;
        
        return *this;
    }

} } }
