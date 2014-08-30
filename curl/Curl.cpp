
#include "Curl.h"

namespace poison { namespace net { namespace http {
    class CurlError : public std::runtime_error {
    public:
        CurlError(CURLcode code_, const std::string& error)
            : std::runtime_error(error)
            , code(code_)
        {

        }

        CURLcode getCode() const { return code; }

    protected:
        CURLcode code;
    };

//            http://stackoverflow.com/questions/2376824/libcurl-http-request-to-save-respond-into-variable-c
    size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
        ((std::string*)stream)->append((char*)ptr, 0, size*count);
        return size*count;
    }

    int curl_debug(CURL *h, curl_infotype it, char *s, size_t sz, void *v) {
        // Not NULL terminated
//        char *buff = new char[sz + 1];
        std::vector<char> buff(sz + 1, '\0');

        strncpy(buff.data(), s, sz);

        if (it == CURLINFO_TEXT) {
            std::cerr << "CURL info: " << buff.data() << std::endl;
        }
        else if (it == CURLINFO_HEADER_IN) {
            std::cerr << "CURL HTTP Header In: " << buff.data() << std::endl;
        }
        else if (it == CURLINFO_HEADER_OUT) {
            std::cerr << "CURL HTTP Header Out: " << buff.data() << std::endl;
        }
        else if (it == CURLINFO_DATA_IN) {
            // Lots of data in, turn on if necessary
        }
        else if (it == CURLINFO_DATA_OUT) {
            // Lots of data out, turn on if necessary
        }
        else {
            // Any others?
        }

        return 0;
    }

    void Curl::CurlCheckError( CURLcode code, const char* errbuff )
    {
        if( code != 0 ){
            throw CurlError(code, errbuff);
        }
    }

    void Curl::send(const Request& request, std::function<void(const Response& response)> onComplete) {
        Response response;

        CURL* curl = curl_easy_init();
        if ( !curl ) {
            throw std::runtime_error("failed to initialize curl");
        }

        char errbuff[CURL_ERROR_SIZE];

        try {


            auto url = request.getUrl();

            std::string postDataString;

            if (!request.getData().empty()) {
                url += "?";
                auto escapedData = request.getData();
                for (auto& pair : escapedData) {

                    auto escapedStr = curl_easy_escape(curl, std::string(pair.second).c_str(), 0);
                    pair.second = escapedStr;
                    curl_free(escapedStr);
                }
                auto getParamsString = Request::implodeMap(escapedData);
                url += getParamsString;
            }

            struct curl_slist *headers = NULL;

            for (const auto& header : request.getHeaders()) {
                    headers = curl_slist_append(headers, header.c_str());
            }

            if ( request.isBinary() || !request.getDataPost().empty() ) {
                // POST query
                curl_easy_setopt(curl, CURLOPT_POST, 1);

                std::string postString;

                // regular post query
                if (!request.getDataPost().empty()) {
                    auto escapedData = request.getDataPost();

                    for (auto& pair : escapedData) {

                        auto escapedStr = curl_easy_escape(curl, std::string(pair.second).c_str(), 0);
                        pair.second = escapedStr;
                        curl_free(escapedStr);
                    }

                    postString = Request::implodeMap(escapedData);
#ifdef NETCLIENT_DEBUG
                    DBG("[NetClient] post params: %s\n", postString.c_str());
#endif
                    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postString.c_str());

                } else {
                    // binary query
                    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, request.getBinaryData().c_str());
                }

            } else {
                curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
            }

            response.request = &request;
#ifdef NETCLIENT_DEBUG
            DBG("[NetClient] query to URL: %s\n", response.url.c_str());
#endif

            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuff);
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, curl_debug), errbuff );
            //        CurlCheckError( curl_easy_setopt(curl, CURLOPT_VERBOSE, true) );
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_URL, request.getUrl().c_str() ), errbuff );
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1 ), errbuff );
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string), errbuff );
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.data), errbuff );
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1), errbuff );
            CurlCheckError( curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false) , errbuff);


            if (headers) {
                CurlCheckError( curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), errbuff );
            }


            setCurlOptions(curl);

            CurlCheckError( curl_easy_perform(curl), errbuff );

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.code);

            if (response.code != 200) {
                // TODO: check this without connection
                response.error = Response::Error::ConnectionError;
            }

            if (headers) {
                curl_slist_free_all(headers);
            }

#ifdef NETCLIENT_DEBUG
            DBG("[NetClient] RESPONSE %s <%d>:\n %s", response.url.c_str(), response.code, response.data.c_str());
#endif

        } catch (const CurlError& e) {
            response.errorText = e.what();

            Response::Error error = Response::Error::NoError ;

            switch ( e.getCode() ) {
                case CURLE_COULDNT_RESOLVE_PROXY:
                case CURLE_COULDNT_RESOLVE_HOST:
                case CURLE_SSL_CONNECT_ERROR:
                case CURLE_COULDNT_CONNECT:
                    error = Response::Error::ConnectionError; break;
                case CURLE_OPERATION_TIMEDOUT:
                    error = Response::Error::TimeoutError; break;
                default:
                    error = Response::Error::InternalError;
            }

            response.error = error;

        } catch (const std::runtime_error& e) {

            response.errorText = e.what();
            response.error = Response::Error::InternalError;

        } catch (...) {

            response.error = Response::Error::InternalError;
            response.errorText = errbuff;

        }

        curl_easy_cleanup( curl );

        if (onComplete)
            onComplete(response);

        onRequestComplete(response);

    }

} } }



