#pragma once

#include "_config.h"
#include "Response.hpp"
#include "DownloaderUtility.hpp"

namespace WebUtils {
    static WEBUTILS_EXPORT DownloaderUtility const downloader{.userAgent = WEBUTILS_USER_AGENT, .timeOut = WEBUTILS_TIMEOUT};

    namespace String {
        /// @brief runs a get string request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<StringResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get string request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(StringResponse)> onFinished);

        /// @brief runs a get string request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        StringResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }

    namespace Data {
        /// @brief runs a get data request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<DataResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get data request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(DataResponse)> onFinished);

        /// @brief runs a get data request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        DataResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }

    #ifdef WEBUTILS_HAS_RAPIDJSON
    namespace Json {
        /// @brief runs a get json request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<JsonResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get json request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(JsonResponse)> onFinished);

        /// @brief runs a get json request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        JsonResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }
    #endif

    #ifdef WEBUTILS_HAS_XML
    namespace XML {
        /// @brief runs a get xml request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<XMLResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get xml request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(XMLResponse)> onFinished);

        /// @brief runs a get xml request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        XMLResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }
    #endif

    #ifdef WEBUTILS_HAS_BSML
    namespace Texture {
        /// @brief runs a get texture request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<TextureResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get texture request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(TextureResponse)> onFinished);

        /// @brief runs a get texture request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        TextureResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }

    namespace Sprite {
        /// @brief runs a get sprite request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<SpriteResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get sprite request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(SpriteResponse)> onFinished);

        /// @brief runs a get sprite request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        SpriteResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }

    namespace BSML {
        /// @brief runs a get bsml request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<BSMLResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions);

        /// @brief runs a get bsml request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(BSMLResponse)> onFinished);

        /// @brief runs a get bsml request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        BSMLResponse WEBUTILS_EXPORT Get(URLOptions urlOptions);
    }
    #endif
}
