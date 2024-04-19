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

    static auto write_vec_cb(uint8_t* content, std::size_t size, std::size_t nmemb, std::vector<uint8_t>* vec) -> std::size_t {
        std::span<uint8_t> addedData(content, (size * nmemb));
        vec->insert(std::next(vec->begin(), vec->size()), addedData.begin(), addedData.end());
        return addedData.size();
    };

    bool DownloaderUtility::GetInto(URLOptions urlOptions, IResponse* response) const {
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
                return response && response->DataParsedSuccesful();
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
        std::string escapedUrl = curl_easy_escape(curl, url.c_str(), url.size());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        curl_easy_setopt(curl, CURLOPT_URL, escapedUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, urlOptions.timeOut.value_or(timeOut));
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        std::vector<uint8_t> data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_vec_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        std::string userAgent = urlOptions.userAgent.value_or(userAgent);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        response->CurlStatus = curl_easy_perform(curl);
        if (response->CurlStatus == CURLE_OK) response->AcceptData(data);

        int httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        response->HttpCode = httpCode;

        curl_easy_cleanup(curl);
        return response && response->DataParsedSuccesful();
    }
}
