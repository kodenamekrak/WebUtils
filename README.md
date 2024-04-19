# WebUtils
A Quest library to simplify downloading various types of content through simple method calls, allowing sync and async method calls

Currently only supports get requests, post requests should be possible though

# Downloadable types
 - String as `std::string`
 - Data as `std::vector<uint8_t>`
 - Json as `rapidjson::Document` (requires bs hook)
 - XML as `tinyxml2::XMLDocument` (requires tinyxml2)
 - BSML as `std::shared_ptr<BSML::BSMLDocParser>` (requires bsml)
 - Texture2D as `UnityW<UnityEngine::Texture2D>` (requires bsml)
 - Sprite as `UnityW<UnityEngine::Sprite>` (requires bsml)

Some of these require bsml because unity requires unity types to be created on main thread.

## Extendable downloads
You should be able to implement all sort of downloadable types (even your own classes!) by utilizing the `WebUtils::IResponse` class (or `WebUtils::GenericResponse<T>`) to implement a basic class that can be used by WebUtils to download data and parse it into a response. this way you needn't be concerned with invoking curl manually and only worry about implementing a few methods. here's an example implementation of something that parses the returned data as an integer:

```c++
namespace MyMod {
    struct IntegerResponse : public WebUtils::GenericResponse<int> {
        virtual bool AcceptData(std::span<uint8_t const> data) {
            // if data not long enough, return false
            if (data.size() < 4) return false;
            // parse the data as an int
            responseData = *(int32_t*)data.data();
            return true;
        }
    };
}
```

If you're not sure how this is done, I would advise you to have a look over the `web-utils/shared/Response.hpp` header and looking at how webutils has implemented various types

## Disable certain types
If you do not want to expose certain methods from the library to your code (for example, you're getting linker errors because the required library is not linked directly) you can do so with various compile time defines:

 - `WEBUTILS_NO_JSON` disables the rapidjson downloading
 - `WEBUTILS_NO_XML` disables the xml downloading
 - `WEBUTILS_NO_BSML` disables the sprite, texture and bsml downloading
