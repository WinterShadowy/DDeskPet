#pragma once

#if defined(__cplusplus) && __cplusplus >= 201402L	// C++14 and above: use standard attribute
#define DEPRECATED(msg) [[deprecated(msg)]]
#elif defined(__GNUC__) || defined(__clang__)	// GCC/Clang extension
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER) // MSVC extension
#define DEPRECATED(msg) __declspec(deprecated(msg))
#else	// Unknown compiler, ignore
#define DEPRECATED(msg)
#endif

#ifndef _LAPP_LOG_SYSTEM
#define _LAPP_LOG_SYSTEM
#endif

// #define _LAPP_NO_LOG_SYSTEM

#if (defined(_LAPP_NO_LOG_SYSTEM) && defined(_LAPP_LOG_SYSTEM))
#undef _LAPP_LOG_SYSTEM
#endif

// 如果定义了_LAPP_NO_LOG_SYSTEM
// 就不定义_LAPP_LOG_SYSTEM_SPDLOG和_LAPP_LOG_SYSTEM_LAPPRUNTIMELOG
#if (defined(_LAPP_NO_LOG_SYSTEM) && defined(_LAPP_LOG_SYSTEM_SPDLOG) && defined(_LAPP_LOG_SYSTEM_LAPPRUNTIMELOG))
#undef _LAPP_LOG_SYSTEM_SPDLOG
#undef _LAPP_LOG_SYSTEM_LAPPRUNTIMELOG
#endif