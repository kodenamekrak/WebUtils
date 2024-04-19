#pragma once

#define WEBUTILS_EXPORT __attribute__((visibility("default")))

#ifdef __cplusplus
#define WEBUTILS_EXPORT_FUNC extern "C" WEBUTILS_EXPORT
#else
#define WEBUTILS_EXPORT_FUNC WEBUTILS_EXPORT
#endif

// check if json parsing is available
#if !defined(WEBUTILS_NO_JSON) && __has_include("beatsaber-hook/shared/config/rapidjson-utils.hpp")
#define WEBUTILS_HAS_RAPIDJSON
#endif

// check if xml parsing is available
#if !defined(WEBUTILS_NO_XML) && __has_include("tinyxml2/shared/tinyxml2.h")
#define WEBUTILS_HAS_XML
#endif

// if no sprite/tex, just ignore bsml availability
#if defined(WEBUTILS_NO_SPRITE) || defined(WEBUTILS_NO_TEXTURE)
#define WEBUTILS_NO_BSML
#endif

// check if bsml parsing is available
#if !defined(WEBUTILS_NO_BSML) && (__has_include("bsml/shared/BSML/MainThreadScheduler.hpp") || __has_include("BSML/MainThreadScheduler.hpp")) && __has_include("bsml/shared/BSML/Parsing/BSMLParser.hpp") &&__has_include("bsml/shared/Helpers/utilities.hpp") && __has_include("UnityEngine/Sprite.hpp") && __has_include("UnityEngine/Texture2D.hpp")
#define WEBUTILS_HAS_BSML
#endif

#ifndef WEBUTILS_USER_AGENT
#if defined(MOD_ID) && defined(VERSION)
#define WEBUTILS_USER_AGENT MOD_ID "/" VERSION " (+https://github.com/RedBrumbler/WebUtils)"
#else
#define WEBUTILS_USER_AGENT "WebUtils (+https://github.com/RedBrumbler/WebUtils)"
#endif
#endif

#ifndef WEBUTILS_TIMEOUT
#define WEBUTILS_TIMEOUT 10
#endif
