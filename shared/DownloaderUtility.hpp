#pragma once

#include "_config.h"
#include "Response.hpp"
#include <future>
#include <thread>
#include <iterator>
#include <type_traits>

namespace WebUtils {
    struct WEBUTILS_EXPORT URLOptions {
        using QueryMap = std::unordered_map<std::string, std::string>;
        using HeaderMap = std::unordered_map<std::string, std::string>;

        URLOptions(std::string url, QueryMap queries = {}, HeaderMap headers = {}, std::optional<std::string> userAgent = std::nullopt, std::optional<int> timeOut = std::nullopt) : url(url), queries(queries), headers(headers), userAgent(userAgent), timeOut(timeOut) {}
        URLOptions(std::string url, QueryMap queries = {}, std::optional<std::string> userAgent = std::nullopt, std::optional<int> timeOut = std::nullopt) : url(url), queries(queries), headers({}), userAgent(userAgent), timeOut(timeOut) {}
        URLOptions(std::string url, std::optional<std::string> userAgent = std::nullopt, std::optional<int> timeOut = std::nullopt) : url(url), queries({}), headers({}), userAgent(userAgent), timeOut(timeOut) {}

        /// @brief base url to request from
        std::string url;
        /// @brief queries to append to the URL
        QueryMap queries;
        /// @brief headers to set on the request
        HeaderMap headers;
        /// @brief userAgent to use for the request, if not set uses the downloader utility set useragent
        std::optional<std::string> userAgent;
        /// @brief timeout to use for the request, if not set uses the downloader utility set timeout
        std::optional<int> timeOut;

        /// @brief formats the url from the set url & queries
        std::string fullURl() const;

        /// @brief checks whether this is a file URL, useful to know
        constexpr bool isFileURL() const noexcept { return url.starts_with("file://"); }
    };

    struct WEBUTILS_EXPORT DownloaderUtility {
        public:
            std::string userAgent;
            int timeOut;

#pragma region GET
            /// @brief generic get for IResponse classes
            /// @return whether there was data & it was parsed successfully
            template<response_impl T>
            requires(std::is_default_constructible_v<T>)
            std::future<T> GetAsync(URLOptions urlOptions) const {
                return std::async(std::launch::any, &DownloaderUtility::Get<T>, this, std::forward<URLOptions>(urlOptions));
            }

            /// @brief generic async get for IResponse classes
            /// @param urlOptions the url options to pass to curl
            template<response_impl T>
            requires(std::is_default_constructible_v<T>)
            void GetAsync(URLOptions urlOptions, std::function<void(T)> onFinished) const {
                if (!onFinished) return;

                std::thread([this](URLOptions urlOptions, std::function<void(T)> onFinished){
                    onFinished(Get<T>(urlOptions));
                }, std::forward<URLOptions>(urlOptions), std::forward<std::function<void(T)>>(onFinished)).detach();
            }

            template<response_impl T>
            requires(std::is_default_constructible_v<T>)
            T Get(URLOptions urlOptions) const {
                T response{};
                GetInto(std::forward<URLOptions>(urlOptions), &response);
                return response;
            }

            /// @brief gets data from a url synchronously
            /// @param urlOptions the url options to pass to curl
            /// @param targetResponse response to get into
            /// @return data parsed successfully
            bool GetInto(URLOptions urlOptions, IResponse* targetResponse) const;

            /// @brief generic get for IResponse classes
            /// @return whether there was data & it was parsed successfully
            std::future<bool> GetAsyncInto(URLOptions urlOptions, IResponse* targetResponse) const {
                return std::async(std::launch::any, &DownloaderUtility::GetInto, this, std::forward<URLOptions>(urlOptions), targetResponse);
            }
#pragma endregion // GET

#pragma region POST
            /// @brief generic async post method
            /// @param urlOptions the url options to pass to curl
            /// @param data the data to send. make sure it lives longer than the request takes!
            /// @return future response
            template<typename T = void>
            requires((response_impl<T> && std::is_default_constructible_v<T>) || std::is_same_v<T, void>)
            std::future<T> PostAsync(URLOptions urlOptions, std::span<uint8_t const> data) const {
                return std::async(std::launch::any, &DownloaderUtility::Post<T>, this, std::forward<URLOptions>(urlOptions), std::forward<std::span<uint8_t const>>(data));
            }

            /// @brief generic async get for IResponse classes
            /// @param urlOptions the url options to pass to curl
            /// @param data the data to send. make sure it lives longer than the request takes!
            /// @param onFinished method called with the result of the post request, if null the request doesn't happen
            template<response_impl T>
            requires(std::is_default_constructible_v<T>)
            void PostAsync(URLOptions urlOptions, std::span<uint8_t const> data, std::function<void(T)> onFinished) {
                if (!onFinished) return;

                std::thread([this](URLOptions urlOptions, std::span<uint8_t const> data, std::function<void(T)> onFinished){
                    onFinished(Post<T>(urlOptions, data));
                }, std::forward<URLOptions>(urlOptions), std::forward<std::span<uint8_t const>>(data), std::forward<std::function<void(T)>>(onFinished)).detach();
            }

            /// @brief generic post method
            /// @tparam T expected response type
            /// @param urlOptions the url options to pass to curl
            /// @param data the data to send.
            /// @return request response
            template<response_impl T>
            requires(std::is_default_constructible_v<T>)
            T Post(URLOptions urlOptions, std::span<uint8_t const> data) const {
                T response{};
                PostInto(urlOptions, data, &response);
                return response;
            }

            /// @brief posts to a url synchronously
            /// @param urlOptions the url options to pass to curl
            /// @param data the data to send.
            /// @param targetResponse post responses may contain response data, this is where it gets parsed into
            /// @return data parsed successfully
            bool PostInto(URLOptions urlOptions, std::span<uint8_t const> data, IResponse* targetResponse) const;

            /// @brief posts to a url async
            /// @param urlOptions the url options to pass to curl
            /// @param data the data to send. make sure it lives longer than the request takes!
            /// @param targetResponse post responses may contain response data, this is where it gets parsed into
            /// @return data parsed successfully
            std::future<bool> PostAsyncInto(URLOptions urlOptions, std::span<uint8_t const> data, IResponse* targetResponse) {
                return std::async(std::launch::any, &DownloaderUtility::PostInto, this, std::forward<URLOptions>(urlOptions), std::forward<std::span<uint8_t const>>(data), std::forward<IResponse*>(targetResponse));
            }
#pragma endregion // POST
    };
}
