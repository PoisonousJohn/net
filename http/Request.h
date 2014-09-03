//
//  NetClientRequest.h
//  match3
//
//  Created by Fateev on 19.12.13.
//  Copyright (c) 2013 Game Insight. All rights reserved.
//

#ifndef __match3__NetClientRequest__
#define __match3__NetClientRequest__

#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>


namespace poison { namespace net { namespace http {
    class Request {
    public:
        typedef std::unordered_map<std::string, std::string> RequestData;
        typedef std::vector<std::string> Headers;

        /**
        * @brief use this constructor to send key-value data\n
        * GET and POST data will be converted to key-value string
        *
        * @param url url without get query
        *
        * @param getData GET data map - will be converted to key-value string
        *
        * @param postData POST data map - will be converted to key-value string
        */
        Request(const std::string& url, const RequestData& getData = RequestData(), const RequestData& postData = RequestData());

        /**
        * @brief use this constructor to send binary (already encoded) data\n
        * to the server. binary data will be sended RAW in POST
        *
        * @param url url without get query
        *
        * @param getData GET data map - will be converted to key-value string
        *
        * @param binaryData raw data to send
        */
        Request(const std::string &url, const std::string& binaryData, const RequestData& getData = RequestData());

        Request(const Request& rvalue);

        Request(const Request&& rvalue);

        Request& operator=(const Request& rvalue);

        Request& operator=(const Request&& rvalue);

        virtual ~Request() {}
        
        void setQueryString(const std::string& value) const { queryString = value; }
        
        /**
         * @return string - url + query string
         **/
        std::string getFullUrl() const { return queryString.empty() ? url : url + "?" + queryString; }

        const std::string& getUrl() const { return url; }

        const std::string& getBinaryData() const { return binary; }

        bool isPost() const { return post; }

        const RequestData& getData() const { return dataGet; }

		const RequestData& getDataPost() const { return dataPost; }

        void setHeaders( const Headers& value ) { headers = value; }

        const Headers& getHeaders() const { return headers; }

        /**
        * @brief use to implode get and post params to key-value query string
        * @detail default parameters for keyValueSeparator and pairSeparator match HTTP standard
        */
        template <typename T>
        static std::string implodeMap(
                const T& map,
                const std::string& keyValueSeparator = "=",
                const std::string& pairSeparator = "&")
        {
            std::string result;

            bool first = true;

            for (auto& pair : map) {
                if (!first)
                    result += pairSeparator;
                else
                    first = false;

                result += pair.first + keyValueSeparator + std::string(pair.second);
            }

            return result;
        }

    protected:
        
        bool post;

        Headers headers;

        std::string url;
        
        mutable std::string queryString;

        std::string binary;

        RequestData dataGet;

		RequestData dataPost;
    };
} } }

#endif /* defined(__match3__NetClientRequest__) */
