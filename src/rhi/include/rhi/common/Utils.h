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
}