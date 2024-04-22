#pragma once

#include "_config.h"
#include "Response.hpp"
#include "DownloaderUtility.hpp"
#include <type_traits>

namespace WebUtils {
    static WEBUTILS_EXPORT DownloaderUtility const downloader{.userAgent = WEBUTILS_USER_AGENT, .timeOut = WEBUTILS_TIMEOUT};

#pragma region GET
    /// @brief runs a get request asynchronously
    /// @tparam T the response type to output
    /// @param urlOptions url options to pass to curl
    /// @return future response
    template<response_impl T>
    requires(std::is_default_constructible_v<T>)
    inline std::future<T> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
        return downloader.GetAsync<T>(std::forward<URLOptions>(urlOptions));
    }

    /// @brief runs a get request asynchronously, calling onFinished when done
    /// @tparam T the response type to output
    /// @param urlOptions url options to pass to curl
    /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
    template<response_impl T>
    requires(std::is_default_constructible_v<T>)
    inline void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(T)> onFinished) {
        downloader.GetAsync<T>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(T)>>(onFinished));   \
    }

    /// @brief runs a get request synchronously
    /// @tparam T the response type to output
    /// @param urlOptions url options to pass to curl
    /// @return response
    template<response_impl T>
    requires(std::is_default_constructible_v<T>)
    inline T WEBUTILS_EXPORT Get(URLOptions urlOptions) {
        return downloader.Get<T>(std::forward<URLOptions>(urlOptions));
    }
#pragma endregion // GET

#pragma region POST
    /// @brief runs a get request asynchronously
    /// @tparam T the response type to output
    /// @param urlOptions url options to pass to curl
    /// @param data the data to send. make sure it lives longer than the request takes!
    /// @return future response
    template<response_impl T>
    requires(std::is_default_constructible_v<T>)
    inline std::future<T> WEBUTILS_EXPORT PostAsync(URLOptions urlOptions, std::span<uint8_t const> data) {
        return downloader.PostAsync<T>(std::forward<URLOptions>(urlOptions));
    }

    /// @brief runs a get request asynchronously, calling onFinished when done
    /// @tparam T the response type to output
    /// @param urlOptions url options to pass to curl
    /// @param data the data to send. make sure it lives longer than the request takes!
    /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
    template<response_impl T>
    requires(std::is_default_constructible_v<T>)
    inline void WEBUTILS_EXPORT PostAsync(URLOptions urlOptions, std::span<uint8_t const> data, std::function<void(T)> onFinished) {
        downloader.PostAsync<T>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(T)>>(onFinished));   \
    }

    /// @brief runs a get request synchronously
    /// @tparam T the response type to output
    /// @param urlOptions url options to pass to curl
    /// @param data the data to send.
    /// @return response
    template<response_impl T>
    requires(std::is_default_constructible_v<T>)
    inline T WEBUTILS_EXPORT Post(URLOptions urlOptions, std::span<uint8_t const> data) {
        return downloader.Post<T>(std::forward<URLOptions>(urlOptions));
    }
#pragma endregion // POST

    namespace String {
        /// @brief runs a get string request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        inline std::future<StringResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<StringResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get string request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        inline void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(StringResponse)> onFinished) {
            downloader.GetAsync<StringResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(StringResponse)>>(onFinished));   \
        }

        /// @brief runs a get string request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        inline StringResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<StringResponse>(std::forward<URLOptions>(urlOptions));
        }
    }

    namespace Data {
        /// @brief runs a get data request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        inline std::future<DataResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<DataResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get data request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        inline void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(DataResponse)> onFinished) {
            downloader.GetAsync<DataResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(DataResponse)>>(onFinished));   \
        }

        /// @brief runs a get data request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        inline DataResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<DataResponse>(std::forward<URLOptions>(urlOptions));
        }
    }

    #ifdef WEBUTILS_HAS_RAPIDJSON
    namespace Json {
        /// @brief runs a get json request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<JsonResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<JsonResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get json request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(JsonResponse)> onFinished) {
            downloader.GetAsync<JsonResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(JsonResponse)>>(onFinished));   \
        }

        /// @brief runs a get json request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        JsonResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<JsonResponse>(std::forward<URLOptions>(urlOptions));
        }
    }
    #endif

    #ifdef WEBUTILS_HAS_XML
    namespace XML {
        /// @brief runs a get xml request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<XMLResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<XMLResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get xml request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(XMLResponse)> onFinished) {
            downloader.GetAsync<XMLResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(XMLResponse)>>(onFinished));   \
        }

        /// @brief runs a get xml request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        XMLResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<XMLResponse>(std::forward<URLOptions>(urlOptions));
        }
    }
    #endif

    #ifdef WEBUTILS_HAS_BSML
    namespace Texture {
        /// @brief runs a get texture request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<TextureResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<TextureResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get texture request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(TextureResponse)> onFinished) {
            downloader.GetAsync<TextureResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(TextureResponse)>>(onFinished));   \
        }

        /// @brief runs a get texture request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        TextureResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<TextureResponse>(std::forward<URLOptions>(urlOptions));
        }
    }

    namespace Sprite {
        /// @brief runs a get sprite request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<SpriteResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<SpriteResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get sprite request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(SpriteResponse)> onFinished) {
            downloader.GetAsync<SpriteResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(SpriteResponse)>>(onFinished));   \
        }

        /// @brief runs a get sprite request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        SpriteResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<SpriteResponse>(std::forward<URLOptions>(urlOptions));
        }
    }

    namespace BSML {
        /// @brief runs a get bsml request asynchronously
        /// @param urlOptions url options to pass to curl
        /// @return future response
        std::future<BSMLResponse> WEBUTILS_EXPORT GetAsync(URLOptions urlOptions) {
            return downloader.GetAsync<BSMLResponse>(std::forward<URLOptions>(urlOptions));
        }

        /// @brief runs a get bsml request asynchronously, calling onFinished when done
        /// @param urlOptions url options to pass to curl
        /// @param onFinished function to run when done, NOT RAN ON MAIN OR BOUND IL2CPP THREAD
        void WEBUTILS_EXPORT GetAsync(URLOptions urlOptions, std::function<void(BSMLResponse)> onFinished) {
            downloader.GetAsync<BSMLResponse>(std::forward<URLOptions>(urlOptions), std::forward<std::function<void(BSMLResponse)>>(onFinished));   \
        }

        /// @brief runs a get bsml request synchronously
        /// @param urlOptions url options to pass to curl
        /// @return response
        BSMLResponse WEBUTILS_EXPORT Get(URLOptions urlOptions) {
            return downloader.Get<BSMLResponse>(std::forward<URLOptions>(urlOptions));
        }
    }
    #endif
}
