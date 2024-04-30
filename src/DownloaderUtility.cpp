#include "DownloaderUtility.hpp"

#include "libcurl/shared/curl.h"
#include "libcurl/shared/easy.h"
#include <fmt/core.h>
#include <filesystem>
#include <fstream>

namespace WebUtils {
    std::string URLOptions::fullURl() const {
        if (queries.empty()) return url;
        std::vector<std::string> formattedQueries;
        for (auto& [key, value] : queries) formattedQueries.emplace_back(fmt::format("{}={}", key, value));
        return fmt::format("{}?{}", url, fmt::join(formattedQueries, "&"));
    }

    static std::size_t write_vec_cb(uint8_t* content, std::size_t size, std::size_t nmemb, std::vector<uint8_t>* vec) {
        std::span<uint8_t> addedData(content, (size * nmemb));
        vec->insert(std::next(vec->begin(), vec->size()), addedData.begin(), addedData.end());
        return addedData.size();
    };

    static std::size_t write_str_cb(char* content, std::size_t size, std::size_t nmemb, std::string* str) {
        std::string_view addedText(content, (size * nmemb));
        str->append(addedText);
        return addedText.size();
    };

    bool DownloaderUtility::GetInto(URLOptions urlOptions, IResponse* response, std::function<void(float)> progressReport) const {
        if (!response) return false;

        // if the url is for a filepath, read it from disk instead
        if (urlOptions.isFileURL()) {
            response->CurlStatus = 0;
            std::filesystem::path filePath(urlOptions.url.substr(7));
            if (std::filesystem::exists(filePath) && filePath.has_filename()) {
                std::ifstream file(filePath, std::ios::ate | std::ios::binary | std::ios::in);
                std::vector<uint8_t> data(file.tellg());
                file.seekg(0, std::ios::beg);
                file.read((char*)data.data(), data.size());
                response->AcceptData(data);
                return response->IsSuccessful() && response->DataParsedSuccessful();
            } else {
                response->HttpCode = 404;
                return false;
            }
        }

        auto curl = curl_easy_init();
        struct curl_slist* curl_headers = nullptr;
        for (const auto& [key, value] : urlOptions.headers) {
            curl_headers = curl_slist_append(curl_headers, fmt::format("{}: {}", key, value).c_str());
        }

        auto url = urlOptions.fullURl();
        auto escapedUrl = curl_easy_escape(curl, url.c_str(), url.size());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        curl_easy_setopt(curl, CURLOPT_URL, escapedUrl);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, urlOptions.timeOut.value_or(timeOut));
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, urlOptions.encoding.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        if (progressReport != nullptr) {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressReport);
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, +[](std::function<void(float)>* progressReport, curl_off_t dltotal, curl_off_t dlnow, curl_off_t utotal, curl_off_t unow){
                auto& func = *progressReport;
                // progress for get is the download values
                float progress = (float)dltotal / (float)dlnow;
                if (std::isnan(progress)) progress = 0.0f;
                func(progress);
                return 0;
            });
        }

        std::vector<uint8_t> recvData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_vec_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recvData);

        std::string recvHeaders;
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_str_cb);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &recvHeaders);

        std::string userAgent = urlOptions.userAgent.value_or(userAgent);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, urlOptions.useSSL);

        response->CurlStatus = curl_easy_perform(curl);
        free(escapedUrl);

        if (response->CurlStatus == CURLE_OK) response->AcceptData(recvData);
        response->AcceptHeaders(recvHeaders);

        int httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        response->HttpCode = httpCode;

        curl_easy_cleanup(curl);
        return response->IsSuccessful() && response->DataParsedSuccessful();
    }

    bool DownloaderUtility::PostInto(URLOptions urlOptions, std::span<uint8_t const> data, IResponse* response, std::function<void(float)> progressReport) const {
        // if the url is for a filepath, write it to disk instead
        if (urlOptions.isFileURL()) {
            std::filesystem::path filePath(urlOptions.url.substr(7));
            if (response) response->CurlStatus = 0;

            if (filePath.has_filename()) {
                // if it exists, delete
                if (std::filesystem::exists(filePath)) {
                    std::filesystem::remove(filePath);
                }

                // write out
                std::ofstream file(filePath, std::ios::binary | std::ios::out);
                file.write((char*)data.data(), data.size());

                // response will just get 0 length return
                if (response) {
                    response->AcceptData(std::span<uint8_t, 0>());
                    return response->IsSuccessful() && response->DataParsedSuccessful();
                } else {
                    return true;
                }
            } else {
                if (response) response->HttpCode = 404;
                return false;
            }
        }

        auto curl = curl_easy_init();
        struct curl_slist* curl_headers = nullptr;
        for (const auto& [key, value] : urlOptions.headers) {
            curl_headers = curl_slist_append(curl_headers, fmt::format("{}: {}", key, value).c_str());
        }

        auto url = urlOptions.fullURl();
        auto escapedUrl = curl_easy_escape(curl, url.c_str(), url.size());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        curl_easy_setopt(curl, CURLOPT_URL, escapedUrl);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, urlOptions.timeOut.value_or(timeOut));
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, urlOptions.encoding.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char*)data.data());

        if (progressReport != nullptr) {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressReport);
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, +[](std::function<void(float)>* progressReport, curl_off_t dltotal, curl_off_t dlnow, curl_off_t utotal, curl_off_t unow){
                auto& func = *progressReport;
                // progress for post is the upload values
                float progress = (float)utotal / (float)unow;
                if (std::isnan(progress)) progress = 0.0f;
                func(progress);
                return 0;
            });
        }

        std::vector<uint8_t> recvData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_vec_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recvData);

        std::string recvHeaders;
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_str_cb);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &recvHeaders);

        std::string userAgent = urlOptions.userAgent.value_or(userAgent);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, urlOptions.useSSL ? 1 : 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, urlOptions.useSSL ? 2 : 0);

        auto curlStatus = curl_easy_perform(curl);
        free(escapedUrl);

        int httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (response) {
            response->CurlStatus = curlStatus;
            response->HttpCode = httpCode;

            if (response->CurlStatus == CURLE_OK) response->AcceptData(recvData);
            response->AcceptHeaders(recvHeaders);

            curl_easy_cleanup(curl);
            return response->IsSuccessful() && response->DataParsedSuccessful();
        }

        curl_easy_cleanup(curl);
        return curlStatus == CURLE_OK;
    }
}
