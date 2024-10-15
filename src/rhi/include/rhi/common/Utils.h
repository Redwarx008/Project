#pragma once

#include <sstream>

namespace rhi
{
    inline void combineStrSS(std::stringstream& ss)
    {
    }

    template <typename ArgType>
    void combineStrSS(std::stringstream& ss, const ArgType& Arg)
    {
        ss << Arg;
    }

    template <typename FirstArgType, typename... RemainArgsType>
    void combineStrSS(std::stringstream& ss, const FirstArgType& firstArg, const RemainArgsType&... remainArgs)
    {
        combineStrSS(ss, firstArg);
        combineStrSS(ss, remainArgs...); // recursive call using pack expansion syntax
    }

    template <typename... ArgsType>
    std::string combineString(const ArgsType&... remainArgs)
    {
        std::stringstream ss;
        combineStrSS(ss, remainArgs...);
        return ss.str();
    }

    // A type cast that is safer than static_cast in debug builds, and is a simple static_cast in release builds.
// Used for downcasting various ISomething* pointers to their implementation classes in the backends.
    template <typename T, typename U>
    T checked_cast(U u)
    {
        static_assert(!std::is_same<T, U>::value, "Redundant checked_cast");
#ifdef _DEBUG
        if (!u) return nullptr;
        T t = dynamic_cast<T>(u);
        if (!t) assert(!"Invalid type cast");  // NOLINT(clang-diagnostic-string-conversion)
        return t;
#else
        return static_cast<T>(u);
#endif
    }
}