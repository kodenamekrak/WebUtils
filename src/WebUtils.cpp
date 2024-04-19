#include "WebUtils.hpp"

#define GET_IMPL(type)                                                                                                      \
namespace type {                                                                                                            \
    std::future<type##Response> GetAsync(URLOptions urlOptions) {                                                           \
        return downloader.GetAsync<type##Response>(std::forward<URLOptions>(urlOptions));                                   \
    }                                                                                                                       \
                                                                                                                            \
    void GetAsync(URLOptions urlOptions, std::function<void(type##Response)> onFinished) {                                  \
        std::thread([](URLOptions urlOptions, std::function<void(type##Response)> onFinished){                              \
            onFinished(downloader.Get<type##Response>(std::forward<URLOptions>(urlOptions)));                               \
        }, std::forward<URLOptions>(urlOptions), std::forward<std::function<void(type##Response)>>(onFinished)).detach();   \
    }                                                                                                                       \
                                                                                                                            \
    type##Response Get(URLOptions urlOptions) {                                                                             \
        return downloader.Get<type##Response>(std::forward<URLOptions>(urlOptions));                                        \
    }                                                                                                                       \
}

namespace WebUtils {
    GET_IMPL(String)
    GET_IMPL(Data)

    #ifdef WEBUTILS_HAS_RAPIDJSON
    GET_IMPL(Json)
    #endif
    #ifdef WEBUTILS_HAS_XML
    GET_IMPL(XML)
    #endif

    #ifdef WEBUTILS_HAS_BSML
    GET_IMPL(Texture)
    GET_IMPL(Sprite)
    GET_IMPL(BSML)
    #endif
}
